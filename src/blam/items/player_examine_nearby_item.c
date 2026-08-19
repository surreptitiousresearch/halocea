/* player_examine_nearby_item @0x836ADE38 — the player has walked over a nearby item object; decide what to
 * do with it. Only acts on an unheld item the unit did not just interact with. In order it:
 *   1. scans the unit's 4 weapon-inventory slots and, via weapon_handle_potential_inventory_item, lets a
 *      matching weapon absorb the item as ammunition (HUD "picked up ammo");
 *   2. if the item is equipment (type 8): a grenade (def type 6) is added to the grenade inventory, other
 *      equipment is either equipped (when none held) or offered as a pickup action;
 *   3. if the item is a weapon (type 4) the unit can use: auto-pick it into a free slot (HUD "picked up
 *      weapon", unzoom, network the swap) or, failing that, offer a weapon-swap action result.
 *
 * NOTE: the equipment-mismatch test (action result 5) compares the item definition's +776 against itself —
 * both operands resolve to *(u16*)(item_definition + 776) (the right-hand TAG_INSTANCE re-derives the same
 * definition pointer). Reproduced verbatim from the binary (disasm-confirmed); the branch never fires. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/unit_add_weapon_mode.h"
#include "headers/object_header_datum.h"
#include "headers/player_datum.h"
#include "headers/unit_datum.h"
#include "headers/item_datum.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_definition_flags.h"
#include "headers/equipment_definition.h"
#include "headers/equipment_powerup_type.h"
#include "headers/global_tag_instances.h"
#include "headers/game_connection.h"
#include "headers/object_type.h"
#include "headers/player_action_result.h"
#include "headers/unit_control_flags.h"
#include "headers/blam_data_globals.h"
#include "headers/networked_set_action_result_type.h"


extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern uint8_t weapon_handle_potential_inventory_item(int weapon_index, int item_index, int16_t local_player_parent_index, int16_t *rounds_picked_up);
extern void hud_picked_up_ammunition(int16_t local_player_index, uint8_t machine_index, int weapon_definition_index, int16_t count);
extern void hud_picked_up_grenade(int16_t local_player_index, uint8_t machine_index, int grenade_definition_index);
extern void hud_picked_up_weapon(int16_t local_player_index, char machine_index, int weapon_definition_index, uint8_t is_client_side);
extern uint8_t unit_add_grenade_to_inventory(int unit_index, int equipment_index);
extern int unit_get_current_equipment(int unit_index);
extern void player_handle_powerup_equipment(int player_index, int equipment_index);
extern void player_set_action_result(int player_index, int16_t action_result, int action_object_index, int16_t action_seat_index);
extern uint8_t unit_can_use_weapon(int unit_index, int weapon_index);
extern int unit_inventory_get_weapon(int unit_index, int16_t index);
extern int16_t unit_get_weapon_count(int unit_index);
extern uint8_t unit_should_autopick_weapon(int unit_index, int weapon_index);
extern uint8_t unit_add_weapon_to_inventory(int unit_index, int weapon_index, int16_t mode);
extern void player_control_unzoom(int unit_index);
extern int16_t game_connection(void);
extern void player_set_action_result_to_network(int player_index, networked_set_action_result_type action_result_type, int action_object_index, int16_t action_result, int16_t action_seat_index, int weapon_to_drop_as_result_of_swap);
extern uint8_t unit_approve_weapon_swap(int unit_index, int new_weapon_index);

void player_examine_nearby_item(int player_index, int item_index)
{
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
    int unit_index = player->unit_index;
    item_datum *item_object = (item_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, item_index)->datum;
    unit_datum *unit_object = (unit_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;

    /* only examine an unheld item that the unit is not already interacting with */
    if (item_object->object.parent_object_index != -1 || item_object->item.ignore_object_index == unit_index)
        return;

    /* 1. let an existing inventory weapon absorb the item as ammunition */
    int matched_weapon = -1;
    int16_t ammo_picked_up = 0;
    for (int slot = 0; slot < 4; slot++)
    {
        int weapon_in_slot = unit_object->unit.weapon_object_indices[slot];
        if (weapon_in_slot != -1
            && weapon_handle_potential_inventory_item(weapon_in_slot, item_index, player->local_player_index,
                                                      &ammo_picked_up))
        {
            matched_weapon = weapon_in_slot;
            break;
        }
    }
    if (matched_weapon != -1 && ammo_picked_up > 0)
        hud_picked_up_ammunition(player->local_player_index,
                                 player->network_player_data.machine_index, /* player byte +100 */
                                 DATA_ARRAY_ELEMENT(object_header_data, object_header_datum,
                                                    matched_weapon)->datum->definition_index,
                                 ammo_picked_up);

    /* 2. equipment (type 8): grenade, equip, or offer a pickup action */
    item_datum *equipment_object = object_try_and_get_and_verify_type(item_index, object_mask_equipment);
    if (equipment_object)
    {
        equipment_definition *equip_definition =
            TAG_GET(equipment_definition, equipment_object->definition_index);
        int16_t equipment_type = equip_definition->equipment.powerup_type;
        if (equipment_type == _powerup_type_grenade)
        {
            if (unit_add_grenade_to_inventory(unit_index, item_index))
                hud_picked_up_grenade(player->local_player_index,
                                      player->network_player_data.machine_index,
                                      equipment_object->definition_index);
        }
        else if (equipment_type)
        {
            if (unit_get_current_equipment(unit_index) == -1)
                player_handle_powerup_equipment(player_index, item_index);
            else if ((uint16_t)equip_definition->equipment.powerup_type
                     != (uint16_t)(TAG_GET(equipment_definition, equipment_object->definition_index))->equipment.powerup_type)
                player_set_action_result(player_index, _player_action_result_swap_for_powerup, item_index, -1);
        }
    }

    /* 3. weapon (type 4) the unit can use: auto-pick or offer a swap */
    item_datum *weapon_object = object_try_and_get_and_verify_type(item_index, object_mask_weapon);
    if (weapon_object && unit_can_use_weapon(unit_index, item_index))
    {
        weapon_definition *new_weapon_definition =
            TAG_GET(weapon_definition, weapon_object->definition_index);

        unsigned int unit_flags = unit_object->unit.control_flags;
        char swap_inhibited_by_unit = ((unit_flags & (1u << _unit_control_weapon_primary_trigger_bit)) != 0
                                       || (unit_flags & (1u << _unit_control_weapon_secondary_trigger_bit)) != 0);

        int current_weapon = unit_inventory_get_weapon(unit_index, unit_object->unit.current_weapon_index);
        int weapon_count = unit_get_weapon_count(unit_index);

        char current_weapon_undroppable = 0;
        if (weapon_count >= 2 && current_weapon != -1
            && (new_weapon_definition->weapon.flags & (1u << _weapon_doesnt_count_toward_maximum_bit)) == 0)
        {
            weapon_definition *current_weapon_definition =
                TAG_GET(weapon_definition, DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, current_weapon)->datum->definition_index);
            current_weapon_undroppable = (current_weapon_definition->weapon.flags & (1u << _weapon_doesnt_count_toward_maximum_bit)) != 0;
        }

        if (!swap_inhibited_by_unit || (new_weapon_definition->weapon.flags & (1u << _weapon_must_be_readied_bit)) == 0)
        {
            if (unit_should_autopick_weapon(unit_index, item_index))
            {
                if (unit_add_weapon_to_inventory(unit_index, item_index, _unit_add_weapon_as_current_weapon))
                {
                    hud_picked_up_weapon(player->local_player_index,
                                         player->network_player_data.machine_index,
                                         item_object->definition_index, 0);
                    player_control_unzoom(unit_index);
                    if (game_connection() == _game_connection_network_server)
                        player_set_action_result_to_network(player_index,
                                                            _networked_action_result_type_weapon_swap,
                                                            item_index,
                                                            _player_action_result_add_weapon_to_inventory, /* action_result value (domain match, param name unverified) */
                                                            -1, -1);
                }
            }
            else if (!current_weapon_undroppable && unit_approve_weapon_swap(unit_index, item_index))
            {
                item_datum *current_weapon_object =
                    object_try_and_get_and_verify_type(current_weapon, object_mask_weapon);
                int16_t action_result;
                if (weapon_count != 1 || !current_weapon_object
                    || current_weapon_object->definition_index == weapon_object->definition_index)
                    action_result = _player_action_result_swap_for_weapon;
                else
                    action_result = _player_action_result_add_weapon_to_inventory;
                player_set_action_result(player_index, action_result, item_index, -1);
            }
        }
    }
}
