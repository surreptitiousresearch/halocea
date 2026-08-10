/* unit_place @0x836D2760 — apply a scenario unit placement's initial state to a freshly-created unit
 * object: seed body/shield vitality from the placement if positive, and if the placement's "already dead"
 * flag (bit 0) is set, ping the death animation and, if the unit's identifier byte (+675) reads 25 (a
 * "corpse" state), strip its weapons, clear its held-object slot, mark it damaged/dead in its flags, look
 * up a death-pose animation frame offset from its animation graph tag, reset vitality/shield, deplete its
 * shield, and recompute its node matrices.
 *
 * DEVIATION: the decompiler's inflated prototype maps `damage_part` to r9 and `alignment_vector` to r10; the
 * registers say otherwise. `angle` (arg7) takes f1 and only *reserves* its GPR shadow r9, so the r9 the
 * caller leaves live (`slwi r9, r8, 2` = 12*unit_index, leftover object_header_data address math at
 * 0x836D2790) is dead — the callee redefines r9 at 0x836D1B40 without ever reading it. The real trailing
 * args are r10 = -1 (body_part sentinel, `li r10, -1` at 0x836D27E4), the stack word at +0x54 = 0 (NULL
 * alignment_vector, 0x836D27F0) and the stack byte at +0x5F = 1 (gate, 0x836D27E8); Xbox 360 parameter
 * slots are 8 bytes wide, so a word lands at slot+4 and a byte at slot+7. unit_handle_forced_player_kill
 * (0x836D6058) and unit_kill_from_network (0x836D57B8) drive the identical +0x54 / +0x5F pair.
 * A prior revision passed 12*unit_index as body_part and 0xFFFFFFFF as alignment_vector; the callee's only
 * guard is a `cmplwi r10, 0` null test (0x836D21E4), so 0xFFFFFFFF walks straight into the dereference. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/_object_definition.h"
#include "headers/animation_graph.h"
#include "headers/animation.h"
#include "headers/scenario_unit_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/real_vector2d.h"
#include "headers/object_damage_flags.h"
#include "headers/object_flags.h"
#include "headers/unit_flags.h"
#include "headers/unit_animation_state.h"
#include "headers/scenario_unit_dead_flags.h"
#include "headers/blam_data_globals.h"


extern void unit_ping_animation(int unit_index, uint8_t died, uint8_t feign_death, uint8_t died_flying, uint8_t ignore_hard_pings, uint8_t force_hard_pings, float angle, int16_t body_part, const real_vector2d *alignment_vector, uint8_t gate);
extern void unit_delete_all_weapons(int unit_index);
extern void object_delete(int object_index);
extern void object_deplete_shield(int object_index);
extern void object_compute_node_matrices_recursive(int object_index);
extern int game_time_get(void);

void unit_place(int unit_index, scenario_unit_datum *scenario_unit)
{
    unit_datum *unit =
        (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;

    _object_definition *unit_definition = TAG_GET(_object_definition, unit->definition_index);

    if ( scenario_unit->body_vitality > 0.0f )
        unit->object.body_vitality = scenario_unit->body_vitality;

    if ( (scenario_unit->flags & (1u << _scenario_unit_dead_bit)) == 0 )
        return;

    unit_ping_animation(unit_index, 1u, 0, 0, 0, 0, 0.0f, -1, nullptr, 1u);

    if ( (unsigned char)unit->unit.animation.state != _unit_state_dying )
        return;

    unit_delete_all_weapons(unit_index);
    unit->unit.grenade_counts[0] = 0;

    int held_object_index = unit->unit.equipment_object_index;
    if ( held_object_index != -1 )
    {
        object_delete(held_object_index);
        unit->unit.equipment_object_index = -1;
    }

    uint16_t new_damage_flags = unit->object.damage_flags | (1u << _object_dead_bit);
    unsigned int new_flags = unit->object.flags | (1u << _object_cannot_be_garbage_bit);

    animation_graph *graph = TAG_GET(animation_graph, unit_definition->animation_graph.index);
    int16_t frame_count =
        ((animation *)graph->animations.address)[unit->object.animation.state.index].frame_count;

    unit->unit.flags |= (1u << _unit_placed_here_dead_bit);
    unit->object.flags = new_flags;
    unit->object.damage_flags = new_damage_flags;
    unit->object.animation.state.frame_index = frame_count < 4 ? 0 : frame_count - 4;
    unit->unit.time_of_death = game_time_get();
    unit->object.body_vitality = 0.0f;
    unit->object.shield_vitality = 0.0f;

    object_deplete_shield(unit_index);
    object_compute_node_matrices_recursive(unit_index);
}
