/* hud_update_nav_point_local_player @0x837EE1B0 — refresh the render state of a local player's four HUD
 * nav-point slots. For each active slot (nav_index set, reference set, not already deactivated) it resolves the
 * target world position by nav-point type — cutscene flag (type 0), object (type 1), or game-engine goal
 * (type 2) — raises it by the slot's z_offset, and asks hud_get_nav_point_render_type how it should draw given
 * the player's head position; the result nibble is packed back into the slot. An object slot whose object has
 * vanished (missing, or flagged being-deleted) is deactivated. Slots that are invalid to begin with, or once the
 * local player has no controlled unit, are left/marked deactivated.
 *
 * The nav-point type is the top nibble of the packing word (the decompiler's __ROL4__(packing,16) >> 28). Object
 * position is read from object.bounding_sphere_center (+160/+164/+168); cutscene-flag position from the
 * scenario cutscene_flags block (92-byte stride, +36/+40/+44); the deletion-pending flag from the word at
 * datum +524 (bit 2) — past object_datum's 500-byte DB layout, kept as faithful residue. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_damage_flags.h"
#include "headers/object_datum.h"
#include "headers/player_datum.h"
#include "headers/hud_nav_point_datum.h"
#include "headers/real_point3d.h"
#include "headers/scenario.h"
#include "headers/scenario_cutscene_flag.h"
#include "headers/object_type.h"
#include "headers/hud_nav_point_type.h"
#include "headers/blam_data_globals.h"


extern int local_player_get_player_index(int16_t local_player_index);
extern void unit_get_head_position(int unit_index, real_point3d *head_position);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern real_point3d *game_engine_get_goal_position(real_point3d *result, int16_t index);
extern int16_t hud_get_nav_point_render_type(int16_t local_player_index, const real_point3d *head_position,
        const real_point3d *target_position, int reference_index);

void hud_update_nav_point_local_player(int16_t local_player_index)
{
    hud_nav_point_player_datum *player_nav = &nav_point_data[local_player_index];

    int player_index = local_player_get_player_index(local_player_index);
    int unit_index = (player_index == -1)
        ? -1
        : DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->unit_index;

    for ( int i = 0; i < MAXIMUM_ACTIVE_NAV_POINTS; i = (int16_t)(i + 1) )
    {
        hud_nav_point_datum *slot = &player_nav->nav_points[i];
        if ( (uint16_t)slot->nav_index == 0xFFFF
          || slot->reference_index == -1
          /* recovered: (slot->packing & 0xF000) == 0xF000 -> slot->type == -1 (deactivated nibble 0xF) */
          || slot->type == -1 )
        {
            /* recovered: slot->packing |= 0xF000 -> slot->type = -1 */
            slot->type = -1;
            continue;
        }

        if ( unit_index == -1 )
            continue;

        int reference_index = -1;
        real_point3d head_position;
        unit_get_head_position(unit_index, &head_position);

        real_point3d target_position;
        /* recovered: slot->packing >> 12 -> slot->type */
        int type = slot->type;
        if ( type == _nav_point_flag )
        {
            scenario_cutscene_flag *flag =
                &((scenario_cutscene_flag *)global_scenario->cutscene_flags.address)[slot->reference_index];
            target_position.n[0] = flag->position.n[0];
            target_position.n[1] = flag->position.n[1];
            target_position.n[2] = flag->position.n[2];
        }
        else if ( type == _nav_point_object )
        {
            /* renamed local: was `object_datum`, which shadowed the type name used below */
            object_datum *object = object_try_and_get_and_verify_type(slot->reference_index,
                                                                                      object_mask_all);
            reference_index = slot->reference_index;
            /* disasm reads *(_WORD*)(obj+262): damage_flags, not int[131] (that landed past the struct). */
            if ( !object || (object->object.damage_flags & (1u << _object_dead_bit)) != 0 )
            {
                slot->reference_index = -1;
                slot->nav_index = -1;
                /* recovered: slot->packing |= 0xF000 -> slot->type = -1 (deactivated nibble 0xF) */
                slot->type = -1;
                continue;
            }
            target_position.n[0] = object->object.bounding_sphere_center.n[0];
            target_position.n[1] = object->object.bounding_sphere_center.n[1];
            target_position.n[2] = object->object.bounding_sphere_center.n[2];
        }
        else if ( type == _nav_point_game_engine_flag )
        {
            real_point3d goal_buffer[9];
            real_point3d *goal_position = game_engine_get_goal_position(goal_buffer, slot->reference_index);
            target_position = *goal_position;
        }

        target_position.n[2] = slot->z_offset + target_position.n[2];
        /* recovered: packing = ((render_type << 8) & 0xF00) | (packing & 0xF0FF) -> screen_type = render_type */
        slot->screen_type = hud_get_nav_point_render_type(local_player_index, &head_position, &target_position,
                                                          reference_index);
    }
}
