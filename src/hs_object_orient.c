/* hs_object_orient @0x837F8070 — scripting command: place and/or aim an object at a scenario cutscene
 * flag. The flag record (92 bytes, in scenario->cutscene_flags) carries a position (+36) and a 2D
 * facing euler (+48). When repositioning an object that is currently attached/seated it is first
 * detached (units exit their seat, other objects detach). The object is reset, then for a unit the
 * facing is written into its aiming vectors (and, when the unit is a player, the player is teleported
 * and the player-control facing is set, transformed into the relevant node space). For non-player
 * objects the position/facing are applied directly via object_set_position. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/unit_datum.h"
#include "headers/player_datum.h"
#include "headers/scenario.h"
#include "headers/scenario_cutscene_flag.h"
#include "headers/real_euler_angles2d.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void unit_exit_seat_end(int object_index, uint8_t is_part_of_non_forced_exit, uint8_t should_allow_clients, uint8_t should_replicate_if_appropriate);
extern void object_detach(int child_object_index);
extern real_vector3d *vector3d_from_euler_angles2d(real_vector3d *vector, const real_euler_angles2d *angles);
extern void object_reset(int object_index);
extern int player_index_from_unit_index(int unit_index);
extern real_matrix4x3 *object_get_node_matrix(int object_index, int16_t node_index);
extern void matrix4x3_inverse(const real_matrix4x3 *matrix, real_matrix4x3 *result);
extern real_vector3d *matrix4x3_transform_normal(const real_matrix4x3 *matrix, const real_vector3d *normal, real_vector3d *result);
extern uint8_t player_teleport(int player_index, int source_unit_index, const real_point3d *position);
extern void player_control_set_facing(int16_t local_player_index, const real_vector3d *forward);
extern void object_set_position(int object_index, const real_point3d *position, const real_vector3d *forward, const real_vector3d *up);

void hs_object_orient(int object_index, int16_t flag_index, uint8_t position, uint8_t facing)
{
    if (object_index == -1)
        return;

    scenario_cutscene_flag *flag =
        &((scenario_cutscene_flag *)global_scenario->cutscene_flags.address)[flag_index];

    object_datum *object_datum_ptr = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;

    /* detach if we are about to reposition a currently attached/seated object */
    if (position && object_datum_ptr->object.parent_object_index != -1)
    {
        if (object_try_and_get_and_verify_type(object_index, object_mask_unit))
            unit_exit_seat_end(object_index, 0, 0, 1u);
        else
            object_detach(object_index);
    }

    real_vector3d flag_facing;
    vector3d_from_euler_angles2d(&flag_facing, &flag->facing);
    object_reset(object_index);

    player_datum *player_datum_ptr = nullptr;
    unit_datum *unit = object_try_and_get_and_verify_type(object_index, object_mask_unit);
    if (unit)
    {
        int local_player_index = player_index_from_unit_index(object_index);

        real_vector3d node_local_facing;
        if (unit->object.parent_object_index == -1)
        {
            node_local_facing = flag_facing;
        }
        else
        {
            const real_matrix4x3 *node_matrix = object_get_node_matrix(unit->object.parent_object_index, unit->object.parent_node_index);
            real_matrix4x3 inverse_node_matrix;
            matrix4x3_inverse(node_matrix, &inverse_node_matrix);
            matrix4x3_transform_normal(&inverse_node_matrix, &flag_facing, &node_local_facing);
        }

        if (facing)
        {
            unit->unit.desired_facing_vector  = flag_facing;
            unit->unit.desired_aiming_vector  = flag_facing;
            unit->unit.desired_looking_vector = flag_facing;
        }

        if (local_player_index != -1)
        {
            player_datum_ptr = DATA_ARRAY_ELEMENT(player_data, player_datum, local_player_index);
            if (position)
                player_teleport(local_player_index, -1, &flag->position);
            if (facing)
            {
                __int16 player_control_index = player_datum_ptr->local_player_index;
                if ((unsigned __int16)player_control_index != 0xFFFF)
                    player_control_set_facing(player_control_index, &node_local_facing);
            }
        }
    }

    /* a player path already applied position/facing; otherwise apply directly */
    const real_vector3d *apply_facing = (facing && !player_datum_ptr) ? &flag_facing : nullptr;
    const real_point3d  *apply_position = (position && !player_datum_ptr)
                                          ? &flag->position : nullptr;
    object_set_position(object_index, apply_position, apply_facing, nullptr);
}
