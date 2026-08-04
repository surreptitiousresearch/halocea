/* unit_postprocess_node_matrices @0x836D0F78 — after a unit's node matrices are animated, apply the inverse-
 * kinematics fix-ups that attach the unit to what it is holding onto. Skipped entirely for unit definitions
 * flagged 0x800 (no IK). For a seated unit (object.seat_index @+672 valid) it looks up the seat's IK descriptor
 * in the tag referenced at definition+0x44, then:
 *   - Seat IK: unless the current pose state (+675) is one of the "detached" animation states, run the seat's
 *     marker-pair IK chain, gripping the parent (seat) object.
 *   - Weapon IK: when a weapon is held (+754 valid) and the pose/hold state permits it, run the per-weapon IK
 *     marker-pair chain gripping the current weapon object, then clear the "needs weapon IK" flag bit (+664 &~1).
 * Each IK entry is a 64-byte pair of 32-char marker names (hand marker @+0, grip marker @+32).
 *
 * DEVIATION: the two pose-state range
 * tests the decompiler renders as goto spaghetti with a leading-zero-count (-1) test are folded into plain
 * comparisons here (cntlzw(x+1)&0x20 != 0 == (x == -1)). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/animation_graph.h"
#include "headers/animation_graph_unit_seat.h"
#include "headers/animation_graph_weapon_class.h"
#include "headers/global_tag_instances.h"
#include "headers/real_matrix4x3.h"
#include "headers/unit_animation_flags.h"
#include "headers/unit_animation_state.h"
#include "headers/unit_definition_flags.h"
#include "headers/blam_data_globals.h"


extern void object_inverse_kinematics(int hand_object_index, char *hand_marker_name, int grip_object_index, char *grip_marker_name, real_matrix4x3 *node_matrices);


void unit_postprocess_node_matrices(int object_index, real_matrix4x3 *node_matrices)
{
    unit_datum *unit = (unit_datum *)(DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum);
    unit_definition *unit_definition = TAG_GET(struct unit_definition, unit->definition_index);

    if ( (unit_definition->unit.flags & (1u << _unit_definition_simple_creature_bit)) != 0 )   /* definition flag: skip IK */
        return;

    int seat_index = (signed char)unit->unit.animation.seat_index;
    if ( seat_index == -1 )
        return;

    animation_graph *ik_tag_data = TAG_GET(animation_graph, unit_definition->object.animation_graph.index);
    animation_graph_unit_seat *seat_block = &((animation_graph_unit_seat *)ik_tag_data->unit_seats.address)[seat_index];
    animation_graph_weapon_class *weapon_ik_block = &((animation_graph_weapon_class *)seat_block->weapon_classes.address)[(signed char)unit->unit.animation.weapon_index];

    if ( unit->object.parent_object_index != -1 )   /* seated */
    {
        int pose_state = (unsigned char)unit->unit.animation.state;
        unsigned __int8 apply_seat_ik = 1;
        switch ( pose_state )
        {
            case _unit_state_hard_ping: case _unit_state_dying_airborne: case _unit_state_dying:
            case _unit_state_entering_seat: case _unit_state_exiting_seat:
            case _unit_state_ai_impulse: case _unit_state_resurrect_front: case _unit_state_resurrect_back:
                apply_seat_ik = 0;
                break;
            default:
                break;
        }

        int seat_ik_count = seat_block->ik_points.count;
        if ( apply_seat_ik && seat_ik_count > 0 )
        {
            char *seat_ik_markers = (char *)seat_block->ik_points.address;
            for ( int i = 0; i < seat_ik_count; i = (__int16)(i + 1) )
            {
                object_inverse_kinematics(object_index, seat_ik_markers + (i << 6), unit->object.parent_object_index,
                        seat_ik_markers + (i << 6) + 32, node_matrices);
            }
        }
    }

    if ( (unsigned __int16)unit->unit.current_weapon_index == 0xFFFF )   /* no current weapon */
        return;

    unsigned int pose_state = (unsigned char)unit->unit.animation.state;
    unsigned __int8 apply_weapon_ik = (unsigned char)unit->unit.animation.action == 0 && unit->unit.animation.soft_ping_animation.index == -1;
    /* suppress weapon IK for: the leap states, every state from hard_ping up through
     * resurrect_back, and the seat-driven flying states */
    if ( pose_state > _unit_state_resurrect_back )
    {
        if ( pose_state >= _unit_state_leap_start && pose_state <= _unit_state_leap_melee )
            apply_weapon_ik = 0;
    }
    else if ( pose_state >= _unit_state_hard_ping
           || (pose_state >= _unit_state_flying_front && pose_state <= _unit_state_flying_right) )
    {
        apply_weapon_ik = 0;
    }

    if ( !apply_weapon_ik )
        return;

    int weapon_ik_count = weapon_ik_block->ik_points.count;
    if ( weapon_ik_count > 0 )
    {
        char *weapon_ik_markers = (char *)weapon_ik_block->ik_points.address;
        for ( int i = 0; i < weapon_ik_count; i = (__int16)(i + 1) )
        {
            int weapon_object_index = -1;
            unit_datum *current = (unit_datum *)(DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum);
            __int16 slot = current->unit.current_weapon_index;
            if ( slot != -1 )
                weapon_object_index = current->unit.weapon_object_indices[slot];
            object_inverse_kinematics(object_index, weapon_ik_markers + (i << 6), weapon_object_index,
                    weapon_ik_markers + (i << 6) + 32, node_matrices);
        }
    }
    unit->unit.animation.flags &= ~(1u << _unit_animation_postpone_weapon_ik_until_interpolation_ends_bit);
}
