/* actor_path_refresh @0x837CA0A8 — recomputes an actor's path toward its currently-assigned destination
 * (an explicit point, a scenario AI-encounter search/firing position, or a prop's pathfinding location,
 * selected by the actor's path-destination "kind" field), then rebuilds the actual path if the
 * destination moved enough and the actor isn't already blocked from pathfinding.
 *
 * Uses whichever of three pathfinding backends applies: `path_3d_build_path` when the actor is a
 * physics-driven ("3d") pathfinder, `cached_path_state` when the caller supplied one, or otherwise builds
 * a fresh one-shot `path_state` on the stack (`actor_path_input_new` + optional target-object/attractor +
 * `path_state_new`/`path_state_destination`/`path_state_find`/`path_state_build_path`).
 *
 * DEVIATION: the decompiler shows the `path_3d_build_path` call's 4th argument as an undefined
 * local never assigned anywhere in the function, and passes `(path_result *)(actor+1160)` as the 5th
 * argument. disasm_range(0x837CA440, 0x837CA4B0) shows the real register mapping is
 * r4=start_point=actor+0x12C(300), f1=avoidance_distance=0.0, r6=end_point=actor+0x488(1160),
 * r7=path=r24=actor+0x4A8(1192) — i.e. the decompiler swapped the 4th/5th argument VALUES (end_point and
 * path), and the value it substituted for that argument is never used at all. `actor+1192` is exactly the
 * same `path_result*` used by both `path_state_build_path` calls elsewhere in this function, confirming
 * the fix. Reconstructed positionally from disasm.
 *
 * DEVIATION: the 4th argument to `path_input_set_attractor` ("object_index") is likewise an
 * undefined local in the decompile; disasm_range(0x837CA440, 0x837CA560) shows it's
 * `*(int *)(actor + 0x28C)` = +652 -> danger_zone.object_index, loaded just before the call but dropped from the
 * decompiler's output.
 *
 * FAITHFUL: the "build a fresh path_state" branch performs a `_RtlCheckStack12` dynamic stack-probe
 * (extending the frame for a 64KB+ `path_state` local) — pure compiler stack-management boilerplate with
 * no source-level effect; reproduced by simply declaring `path_state fresh_path_state` as a plain local.
 * The 2-float stack slot the decompiler shows earlier (as `actor_path_3d_available`'s output parameter) is
 * the SAME stack slot reused for that later `path_state`, in a mutually-exclusive branch (gated by the
 * same byte both times: +153 -> state.flying) — modeled here as two separate, non-conflicting locals. */

#include <stdint.h>
#include <math.h>
#include "headers/actor_definition_flags2_flags.h"

#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/prop_datum.h"
#include "headers/actor_definition.h"
#include "headers/encounter_definition.h"
#include "headers/squad_definition.h"
#include "headers/move_position_definition.h"
#include "headers/firing_position_definition.h"
#include "headers/scenario.h"
#include "headers/structure_bsp.h"
#include "headers/real_point3d.h"
#include "headers/path_input.h"
#include "headers/path_state.h"
#include "headers/path_result.h"
#include "headers/path_debug_storage.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_danger_zone_type.h"
#include "headers/prop_perception_state.h"
#include "headers/destination_type.h"
#include "headers/blam_data_globals.h"


#include "headers/structure_bsp.h"
extern uint8_t actor_test_destination(uint16_t actor_index);
extern uint8_t actor_path_3d_available(uint16_t actor_index, const real_point3d *destination_point, float *avoidance_distance_reference);
extern void actor_perception_find_prop_pathfinding_location(uint16_t actor_index, uint16_t prop_index);
extern void actor_path_input_new(uint16_t actor_index, path_input *input);
extern void path_input_set_target_object(path_input *input, int target_object_index);
extern void path_input_set_attractor(path_input *input, const real_point3d *attractor_point, float radius, int object_index, float weight);
extern uint8_t path_3d_build_path(structure_bsp *structure_bsp, const real_point3d *start_point, float avoidance_distance, const real_point3d *end_point, path_result *path);
extern void path_state_new(const path_input *input, path_state *state, path_debug_storage *debug);
extern void path_state_destination(path_state *state, const real_point3d *destination_point, int destination_surface_index, float destination_accept_radius);
extern uint8_t path_state_build_path(path_state *state, path_result *path);
extern uint8_t path_state_find(path_state *state);
extern void actor_path_clear(uint16_t actor_index);

uint8_t actor_path_refresh(int actor_index, uint8_t new_destination, path_state *cached_path_state)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    /* Path-destination "kind": 0/1 = none/idle, 2 = explicit point, 3/4 = AI-encounter search/firing
     * position (two different encounter sub-tables), 5 = a prop's pathfinding location. */
    int16_t kind = actor->control.path.destination_orders.destination_type;

    unsigned char had_previous_destination = 0;
    real_point3d previous_destination;
    if (kind && kind != 1)
    {
        had_previous_destination = 1;
        previous_destination = actor->control.path.destination.point;
    }

    /* Bail out entirely (destination considered already reached) if: this actor has no path-3d capable
     * pathfinding available (actor+352), has no destination kind at all, is idle (kind==1), or is
     * encounter-based (kind==3) but has quit its encounter (actor+955). */
    if (actor->input.vehicle_passenger || !kind || kind == _destination_halt || (kind == _destination_firing_position && actor->firing_positions.moved_away_from_firing_position))
    {
        actor->control.path.path.valid = 0;
        actor->control.path.at_destination = 1;
        actor->control.path.destination_original_distance = 0.0f;
        return 1;
    }

    actor->control.path.path.valid = 0;
    actor->control.path.at_destination = 0;
    actor->control.movement_complete = 0;
    actor->control.path.destination_original_distance = 0.0f;

    unsigned char target_found = 1;
    if ((unsigned int)(kind - _destination_raw_location) <= 3) /* kind in [_destination_raw_location .. _destination_prop] */
    {
        if (kind == _destination_move_position)
        {
            target_found = 0;
            int encounter_index = actor->meta.encounter_index;
            if (encounter_index != -1)
            {
                int16_t cell_index = actor->control.path.destination_orders.___u3.move_position_index;
                /* BUGFIX vs prior transcription: the decompiler indexed ai_encounters with DWORD
                 * arithmetic (44 dwords = one 176-byte encounter_definition); the raw-offset version
                 * had kept "44" against a char*. */
                squad_definition *squad_def = (squad_definition *)
                        ((encounter_definition *)global_scenario->ai_encounters.address
                         + (uint16_t)encounter_index)->squads.address
                        + actor->meta.squad_index;
                if (cell_index >= 0 && cell_index < squad_def->move_positions.count)
                {
                    target_found = 1;
                    move_position_definition *record =
                            (move_position_definition *)squad_def->move_positions.address + cell_index;
                    actor->control.path.destination.point.x = record->position.x;
                    actor->control.path.destination.point.y = record->position.y;
                    actor->control.path.destination.point.z = record->position.z;
                    actor->control.path.destination.target_radius = 0.0f;
                    actor->control.path.destination.surface_index = record->surface_index;
                }
            }
        }
        else if (kind == _destination_raw_location)
        {
            /* Explicit destination point already staged at actor+1136..1148; copy into the active slot. */
            actor->control.path.destination.target_radius = 0.0f;
            actor->control.path.destination.surface_index = actor->control.path.destination_orders.___u3.raw.surface_index;
            actor->control.path.destination.point.x = actor->control.path.destination_orders.___u3.raw.point.x;
            actor->control.path.destination.point.y = actor->control.path.destination_orders.___u3.raw.point.y;
            actor->control.path.destination.point.z = actor->control.path.destination_orders.___u3.raw.point.z;
        }
        else if (kind == _destination_firing_position)
        {
            int encounter_index = actor->meta.encounter_index;
            if (encounter_index == -1)
            {
                target_found = 0;
            }
            else
            {
                /* same DWORD-stride BUGFIX as the kind==4 branch above */
                firing_position_definition *record = (firing_position_definition *)
                        ((encounter_definition *)global_scenario->ai_encounters.address
                         + (uint16_t)encounter_index)->firing_positions.address
                        + actor->control.path.destination_orders.___u3.firing_position_index;
                actor->control.path.destination.point.x = record->position.x;
                actor->control.path.destination.point.y = record->position.y;
                actor->control.path.destination.point.z = record->position.z;
                int surface_index = record->surface_index;
                actor->control.path.destination.target_radius = 0.0f;
                actor->control.path.destination.surface_index = surface_index;
            }
        }
        else /* kind == _destination_prop: prop pathfinding location */
        {
            int prop_index = (uint16_t)actor->control.path.destination_orders.___u3.prop.prop_index;
            prop_datum *prop = DATUM_GET(prop_data, prop_datum, prop_index);
            int16_t prop_state = prop->state;
            if (prop_state < _prop_state_uninspected_orphan || prop_state > _prop_state_inspected_orphan)
                actor_perception_find_prop_pathfinding_location(actor_index, actor->control.path.destination_orders.___u3.prop.prop_index);

            float destination_z; /* z of the destination real_point3d (actor+1160); staged before the store */
            if (actor->state.flying)
            {
                actor->control.path.destination.point.x = prop->center_of_mass.x;
                actor->control.path.destination.point.y = prop->center_of_mass.y;
                destination_z = prop->center_of_mass.z;
            }
            else
            {
                actor->control.path.destination.point.x = prop->pathfinding_point.x;
                actor->control.path.destination.point.y = prop->pathfinding_point.y;
                destination_z = prop->pathfinding_point.z;
            }
            actor->control.path.destination.point.z = destination_z;
            int destination_surface_index = prop->pathfinding_surface_index;
            actor->control.path.destination.target_radius = actor->control.path.destination_orders.___u3.prop.accept_radius;
            actor->control.path.destination.surface_index = destination_surface_index;
        }
    }
    else
    {
        target_found = 0;
    }

    unsigned char rebuild_result = target_found;
    if (target_found)
    {
        uint8_t should_rebuild = 1;
        if (actor->state.flying)
        {
            float avoidance_distance_out = 0.0f;
            should_rebuild = actor_path_3d_available(actor_index, &actor->control.path.destination.point,
                    &avoidance_distance_out);
        }
        else if (actor->control.path.destination.target_radius == 0.0f)
        {
            /* No accept radius: rebuild whenever the path is exhausted or nearly so. */
            /* DEVIATION: disasm is lwz(surface_index); addi +1; addic/subfe — the carry idiom for
             * (surface_index + 1 != 0), i.e. surface_index != -1; the byte-pun arithmetic was decompiler garbage. */
            should_rebuild = actor->control.path.destination.surface_index != -1;
        }

        if (should_rebuild)
        {
            unsigned char destination_moved = 1;
            if (actor_test_destination(actor_index))
            {
                destination_moved = 0;
                if (!had_previous_destination)
                {
                    destination_moved = 1;
                }
                else
                {
                    real_point3d *destination = &actor->control.path.destination.point;
                    float dx = destination->n[0] - previous_destination.n[0];
                    float dy = destination->n[1] - previous_destination.n[1];
                    float dz = destination->n[2] - previous_destination.n[2];
                    if (dx * dx + dz * dz + dy * dy <= 0.010000001f)
                        destination_moved = 0;
                }
            }

            if (destination_moved)
            {
                real_point3d *destination = &actor->control.path.destination.point;
                real_point3d *actor_position = &actor->input.position.body_position;
                float dx = destination->n[0] - actor_position->n[0];
                float dy = destination->n[1] - actor_position->n[1];
                float dz = destination->n[2] - actor_position->n[2];
                actor_definition *actor_tag = TAG_GET(actor_definition, actor->meta.definition_index);
                float distance_to_destination = (float)sqrt((dx * dx + (dy * dy + dz * dz)));

                path_result *path = &actor->control.path.path;
                if (actor->state.flying)
                {
                    rebuild_result = path_3d_build_path(global_structure_bsp, actor_position, 0.0f,
                            destination, path);
                }
                else if (cached_path_state)
                {
                    path_state_destination(cached_path_state, destination, actor->control.path.destination.surface_index,
                            actor->control.path.destination.target_radius);
                    rebuild_result = path_state_build_path(cached_path_state, path);
                }
                else
                {
                    path_input input;
                    path_state fresh_path_state;

                    actor_path_input_new(actor_index, &input);
                    /* Same field actor_move_to_point.c calls "ignore_target_object_index" at this offset. */
                    int target_object_index = actor->control.path.destination_orders.ignore_target_object_index;
                    if (target_object_index != -1)
                        path_input_set_target_object(&input, target_object_index);
                    if (actor->danger_zone.danger_type > actor_danger_zone_none && !actor->danger_zone.attached_to_us && (actor_tag->flags2 & (1u << _actor_definition_flags2_pathfinding_ignores_danger_bit)) == 0)
                    {
                        int attractor_object_index = actor->danger_zone.object_index;
                        path_input_set_attractor(&input, &actor->danger_zone.position,
                                actor->danger_zone.danger_radius, attractor_object_index, 10.0f);
                    }
                    path_state_new(&input, &fresh_path_state, nullptr);
                    path_state_destination(&fresh_path_state, destination, actor->control.path.destination.surface_index,
                            actor->control.path.destination.target_radius);
                    rebuild_result = 0;
                    if (path_state_find(&fresh_path_state))
                        rebuild_result = path_state_build_path(&fresh_path_state, path) != 0;
                }

                actor->control.path.refreshed_this_tick = 1;
                if (new_destination)
                    actor->control.path.destination_original_distance = distance_to_destination;

                if (rebuild_result && actor->control.path.path.endpoint.target_radius > 0.0f
                        && distance_to_destination < actor->control.path.destination.target_radius
                        && distance_to_destination - actor->control.path.path.endpoint.target_radius < 0.5f)
                {
                    /* DEVIATION: collapsed inlined copy of actor_path_clear@0x837C86D0 (zero-xref) to its call; int actor_index narrows to the callee's uint16_t exactly as the inlined DATA_ARRAY_ELEMENT fetch does. */
                    actor_path_clear(actor_index);
                }
            }
        }
        else
        {
            rebuild_result = 0;
        }
    }

    if (!rebuild_result)
    {
        /* DEVIATION: collapsed inlined copy of actor_path_clear@0x837C86D0 (zero-xref) to its call. */
        actor_path_clear(actor_index);
    }
    return rebuild_result;
}
