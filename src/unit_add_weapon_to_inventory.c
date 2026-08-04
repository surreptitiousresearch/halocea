/* unit_add_weapon_to_inventory @0x836D318C — give a weapon object to a unit, placing it in the first free
 * inventory slot. On a client connection the unit's seat/weapon label is reconciled first (dropping the
 * seat if it no longer matches). The pickup only proceeds if the weapon object is flagged pickup-able
 * (object flag 0x800), is unowned (object.parent_object_index == -1), the unit can use it, and the game
 * engine permits the pickup. mode _unit_add_weapon_as_only_weapon clears the existing inventory first. Once placed, the weapon is
 * disconnected from the map, hidden, and marked as held; the desired weapon is updated depending on mode
 * (_unit_add_weapon_as_current_weapon = select if not already auto-selecting, _unit_add_weapon_normally =
 * advance to next weapon, >= NUMBER_OF_UNIT_ADD_WEAPON_FLAGS = leave selection alone).
 * _unit_add_weapon_as_only_weapon clears the existing inventory first.
 * Returns 1 on a successful add, 0 otherwise. */

#include <stdint.h>
#include "headers/unit_add_weapon_mode.h"
#include "headers/unit_datum.h"
#include "headers/weapon_datum.h"
#include "headers/game_connection.h"
#include "headers/object_flags.h"
#include "headers/unit_control_flags.h"
#include "headers/unit_animation_state.h"
#include "headers/object_type.h"

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern int16_t game_connection(void);
extern const char *unit_get_seat_label(int object_index);
extern const char *weapon_get_label(int weapon_index);
extern uint8_t unit_set_or_test_seat_and_weapon_label(int object_index, const char *seat_label, const char *weapon_label, uint8_t change_flag);
extern void unit_exit_seat_end(int object_index, uint8_t is_part_of_non_forced_exit, uint8_t should_allow_clients, uint8_t should_replicate_if_appropriate);
extern uint8_t unit_can_use_weapon(int unit_index, int weapon_index);
extern uint8_t game_engine_picking_up(int unit_index, int item_index);
extern void unit_delete_all_weapons(int unit_index);
extern void object_disconnect_from_map(int object_index);
extern void object_set_visibility(int object_index, uint8_t visible_flag);
extern void item_in_unit_inventory(int item_index, int owner_unit_index);
extern void player_control_set_desired_weapon(int unit_index, int16_t desired_weapon_index);
extern int16_t unit_weapon_next_index(int unit_index, int16_t current_index, int16_t delta);

uint8_t unit_add_weapon_to_inventory(int unit_index, int weapon_index, int16_t mode)
{
    unit_datum *unit_object = object_try_and_get_and_verify_type(unit_index, object_mask_unit);
    weapon_datum *weapon_object = object_try_and_get_and_verify_type(weapon_index, object_mask_weapon);

    if (game_connection() == _game_connection_network_client) /* client: reconcile seat/weapon label, leave seat on mismatch */
    {
        const char *seat_label = unit_get_seat_label(unit_index);
        const char *weapon_label = weapon_get_label(weapon_index); /* attested: weapon_get_label returns const char* */
        if (!unit_set_or_test_seat_and_weapon_label(unit_index, seat_label, weapon_label, 0))
        {
            unit_datum *unit_recheck = object_try_and_get_and_verify_type(unit_index, object_mask_unit);
            if (unit_recheck && (uint8_t)unit_recheck->unit.animation.state == _unit_state_exiting_seat) /* mid-seat-enter */
                unit_exit_seat_end(unit_index, 1, 1, 0);
        }
    }

    if ((weapon_object->object.flags & (1u << _object_connected_to_map_bit)) == 0   /* object flags: pickup-able */
        || weapon_object->object.parent_object_index != -1 /* not already owned */
        || !unit_can_use_weapon(unit_index, weapon_index)
        || !game_engine_picking_up(unit_index, weapon_index))
        return 0;

    if (mode == _unit_add_weapon_as_only_weapon)
        unit_delete_all_weapons(unit_index);

    /* first empty inventory slot */
    int16_t slot = -1;
    for (int index = 0; index < 4; index++)
    {
        if (unit_object->unit.weapon_object_indices[index] == -1)
        {
            slot = (int16_t)index;
            break;
        }
    }

    if (slot == -1)
        return 0;

    object_disconnect_from_map(weapon_index);
    object_set_visibility(weapon_index, 0);
    item_in_unit_inventory(weapon_index, unit_index);
    unit_object->unit.weapon_object_indices[slot] = weapon_index;
    unit_object->unit.weapon_last_used_at_game_time[slot] = 0;

    if (mode != _unit_add_weapon_normally)
    {
        if (mode == _unit_add_weapon_as_current_weapon)
        {
            if ((unit_object->unit.control_flags & (1u << _unit_control_weapon_primary_trigger_bit)) == 0) /* unit not already auto-selecting */
                player_control_set_desired_weapon(unit_index, slot);
        }
        else if ((unsigned int)mode >= NUMBER_OF_UNIT_ADD_WEAPON_FLAGS)
        {
            return 1;
        }
        unit_object->unit.desired_weapon_index = slot;
        return 1;
    }

    unit_object->unit.desired_weapon_index =
        unit_weapon_next_index(unit_index, unit_object->unit.current_weapon_index, 0);
    return 1;
}
