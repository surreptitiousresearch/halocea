/* player_teleport_internal @0x836AAC78 — attempts to place a player's controlled unit at `position`
 * (biped_fix_position), then, on success, clears the unit's cluster cache, rejects the teleport if it lands
 * back inside a bsp-switch trigger volume for the CURRENT bsp (would immediately re-trigger a switch), and on
 * final success zeroes the unit's velocity, re-faces it along the exited/source object's forward vector
 * (duplicated into three biped-extension slots), and — if this player has an exit/dismount effect defined —
 * spawns it. If the unit was riding another object (source_unit_index's ultimate parent isn't itself), instead
 * of teleporting straight to `position` it searches a ring of candidate points around that parent object (an
 * "exit" search: a facing/up-derived horizontal direction, scaled by the unit's own biped tag's jump-distance
 * value plus the parent's bounding radius, sampled at 9 fixed offsets with up to 8 random nudges each) until
 * one of them is clear. If the search exhausts, the player is pseudo-killed instead.
 *
 * DEVIATION (corrected 2026-07-27): the decompiler renders biped_fix_position's trailing args scrambled by
 * its float-GPR-skip bug (see biped_fix_position.c). By the callee's own prologue (mr r30,r8 / mr r17,r9 /
 * mr r29,r10) the three flags arrive in r8/r9/r10; r7 is the ABI's dead GPR slot consumed by the float arg
 * (an earlier note here mistook stale r7 contents for the 6th argument). All three call sites set
 * li r8,0 / li r9,0 / li r10,1 — i.e. fix_below_new_position=0, dont_teleport=0, use_radius_as_multiplier=1.
 * DEVIATION: the decompiler displays `v41`/`v40` (color/impulse_field) as declared-but-never-assigned locals
 * at the final effect_new_from_object call; disasm shows both are genuine NULL constants (`li r9,0`/`li r10,0`),
 * not garbage — reproduced as NULL.
 * DEVIATION: the decompiler's byte-level tracking of the "duplicate forward vector into three biped-extension
 * slots" copy (via a QWORD-sized intermediate) renders as non-12-byte-aligned offsets (552/556/560/564/596/600/604).
 * Disasm shows the real store offsets are three clean, contiguous real_vector3d-sized copies at +548, +560,
 * and +596 — reproduced in that clean form. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_rgb_color.h"
#include "headers/effect_vector_field.h"
#include "headers/scenario.h"
#include "headers/scenario_bsp_switch_trigger_volume.h"
#include "headers/game_globals_definition.h"
#include "headers/game_globals_player_information.h"
#include "headers/players_globals.h"
#include "headers/player_datum.h"
#include "headers/biped_datum.h"
#include "headers/biped_definition.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

#include "headers/real_rgb_color.h"
#include "headers/effect_vector_field.h"
extern const real_point3d adjustment_weights[9];

extern int object_get_ultimate_parent(int object_index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern uint8_t biped_fix_position(int biped_index, int line_of_site_object_index, const real_point3d *new_position, real_point3d *final_position, float max_radius_fudge_factor, uint8_t fix_below_new_position, uint8_t dont_teleport, uint8_t use_radius_as_multiplier);
extern float normalize3d(real_vector3d *v);
extern void matrix4x3_from_point_and_vectors(real_matrix4x3 *matrix, const real_point3d *point, const real_vector3d *forward, const real_vector3d *up);
extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern uint32_t *get_global_random_seed_address(void);
extern real_vector3d *seed_random_direction3d(uint32_t *seed, real_vector3d *direction);
extern uint8_t scenario_trigger_volume_test_object(int16_t trigger_volume_index, int object_index);
extern void player_control_set_facing(int16_t local_player_index, const real_vector3d *forward);
extern void players_compute_combined_pvs(unsigned int *combined_pvs, uint8_t local_only);
extern int effect_new_from_object(int definition_index, int owner_object_index, int object_index, int16_t force_local_player_index, float scale_a, float scale_b, const real_rgb_color *color, const effect_vector_field *impulse_field);
extern void player_pseudo_kill(int player_index, int source_unit_index);

uint8_t player_teleport_internal(int player_index, int source_unit_index, const real_point3d *position)
{
    uint8_t fixed = 0;

    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
    int unit_index = player->unit_index;

    biped_datum *unit_object = (biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;

    int line_of_site_object_index = source_unit_index;

    if (source_unit_index == -1 || object_get_ultimate_parent(source_unit_index) == source_unit_index)
    {
        fixed = biped_fix_position(unit_index, line_of_site_object_index, position, 0, 2.0f, 0, 0, 1);
    }
    else
    {
        int ultimate_parent = object_get_ultimate_parent(source_unit_index);
        line_of_site_object_index = ultimate_parent;

        unit_datum *parent_object = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, ultimate_parent)->datum;

        float vel_x = parent_object->object.translational_velocity.n[0];
        float vel_y = parent_object->object.translational_velocity.n[1];

        real_vector3d exit_direction;
        exit_direction.n[0] = vel_x;
        exit_direction.n[1] = vel_y;
        exit_direction.n[2] = 0.0f;

        if (vel_x * vel_x + vel_y * vel_y <= 0.0f)
        {
            /* parent isn't moving horizontally: fall back to its up vector if it's facing near-vertical
             * (forward.z >= cos 45 deg), otherwise its forward vector */
            float dx, dy, dz;
            if (parent_object->object.forward.n[2] >= 0.70710677f)
            {
                dx = parent_object->object.up.n[0];
                dy = parent_object->object.up.n[1];
                dz = parent_object->object.up.n[2];
            }
            else
            {
                dx = parent_object->object.forward.n[0];
                dy = parent_object->object.forward.n[1];
                dz = parent_object->object.forward.n[2];
            }
            exit_direction.n[0] = dx;
            exit_direction.n[1] = dy;
            exit_direction.n[2] = dz;
        }
        exit_direction.n[2] = 0.0f; /* always flattened to the horizontal plane */

        float parent_radius = parent_object->object.bounding_sphere_radius;
        biped_definition *unit_tag = TAG_GET(biped_definition, unit_object->definition_index);
        float jump_distance = unit_tag->biped.collision_radius;   /* DB name; used as the exit-search step */

        exit_direction.n[0] = -exit_direction.n[0];
        exit_direction.n[1] = -exit_direction.n[1];
        exit_direction.n[2] = -0.0f;
        float search_radius = jump_distance * 3.0f + parent_radius;
        normalize3d(&exit_direction);

        real_matrix4x3 search_matrix;
        matrix4x3_from_point_and_vectors(&search_matrix,
                                         &parent_object->object.bounding_sphere_center,
                                         &exit_direction, global_up3d);
        search_matrix.scale = search_radius;

        for (__int16 offset_index = 0; !fixed && (unsigned __int16)(offset_index + 1) < 9; offset_index++)
        {
            real_point3d candidate_position;
            matrix4x3_transform_point(&search_matrix, &adjustment_weights[offset_index], &candidate_position);
            fixed = biped_fix_position(unit_index, ultimate_parent, &candidate_position, 0, 2.0f, 0, 0, 1);

            for (int random_attempt = 0; !fixed && random_attempt < 8; random_attempt++)
            {
                real_vector3d random_direction = *global_zero_vector3d;
                seed_random_direction3d(get_global_random_seed_address(), &random_direction);

                real_point3d random_position;
                random_position.n[0] = random_direction.n[0] * jump_distance + candidate_position.n[0];
                random_position.n[1] = random_direction.n[1] * jump_distance + candidate_position.n[1];
                random_position.n[2] = random_direction.n[2] * jump_distance + candidate_position.n[2];

                fixed = biped_fix_position(unit_index, ultimate_parent, &random_position, 0, 2.0f, 0, 0, 1);
            }
        }
    }

    player->cluster_index = -1;

    if (fixed)
    {
        scenario *scenario_data = global_scenario;
        if (scenario_data->bsp_switch_trigger_volumes.count > 0)
        {
            for (int i = 0; i < scenario_data->bsp_switch_trigger_volumes.count; i++)
            {
                scenario_bsp_switch_trigger_volume *trigger_volume =
                    &((scenario_bsp_switch_trigger_volume *)scenario_data->bsp_switch_trigger_volumes.address)[i];
                if (trigger_volume->source_bsp_index == global_structure_bsp_index
                    && scenario_trigger_volume_test_object(trigger_volume->trigger_volume_index, unit_index))
                {
                    fixed = 0;
                    break;
                }
            }
        }
    }

    if (fixed)
    {
        unit_object->object.translational_velocity = *global_zero_vector3d;

        if (line_of_site_object_index != -1)
        {
            unit_datum *exit_object = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, line_of_site_object_index)->datum;

            real_vector3d exit_forward = exit_object->object.forward;

            biped_datum *verified_object = object_try_and_get_and_verify_type(line_of_site_object_index, object_mask_biped);
            if (verified_object)
            {
                int elevator_object_index = verified_object->biped.elevator_object_index;
                if (elevator_object_index != -1)
                {
                    unit_object->biped.elevator_object_index = elevator_object_index;
                    unit_object->biped.elevator_ticks = verified_object->biped.elevator_ticks;
                }
            }

            /* duplicate the exit object's forward vector into the desired facing/aiming/looking vectors */
            unit_object->unit.desired_facing_vector  = exit_forward;
            unit_object->unit.desired_aiming_vector  = exit_forward;
            unit_object->unit.desired_looking_vector = exit_forward;

            __int16 local_player_index = (unsigned __int16)player->local_player_index;
            if (local_player_index != (__int16)0xFFFF)
                player_control_set_facing(local_player_index, &exit_forward);

            int exit_effect_index = ((game_globals_player_information *)global_game_globals->player_information.address)->coop_respawn_effect.index;
            if (exit_effect_index != -1)
            {
                players_compute_combined_pvs(players_globals->combined_pvs, 0);
                effect_new_from_object(exit_effect_index, unit_index, unit_index, -1, 0.0f, 0.0f, 0, 0);
            }
        }
    }
    else
    {
        player_pseudo_kill(player_index, line_of_site_object_index);
    }

    return fixed;
}
