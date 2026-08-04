/* player_examine_nearby_item_client @0x836AC178 — on a client, decide which "action" prompt a player should
 * see for a nearby item their unit could interact with, and record it via player_set_action_result. The item
 * must be unheld (no parent) and not already owned by this unit. Equipment (type 8) can offer a "use" prompt
 * (result 5). A weapon (type 4) the unit can use offers either "pick up" (result 6) or, when the unit already
 * holds a single different weapon, "swap" (result 7) — unless the weapon would be auto-picked, the held weapon
 * forbids swapping, or a dual-wield restriction applies.
 *
 * DEVIATION: in the equipment branch the binary loads the equipment definition's field 0x308 twice and
 * compares it to itself (the decompiler showed the same via aliased temporaries), so result 5 is never set in
 * this build; reproduced verbatim from the disassembly. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/player_datum.h"
#include "headers/unit_datum.h"
#include "headers/item_datum.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_definition_flags.h"
#include "headers/equipment_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/unit_control_flags.h"
#include "headers/object_type.h"
#include "headers/player_action_result.h"
#include "headers/blam_data_globals.h"


extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern int unit_get_current_equipment(int unit_index);
extern uint8_t unit_can_use_weapon(int unit_index, int weapon_index);
extern uint8_t unit_should_autopick_weapon(int unit_index, int weapon_index);
extern uint8_t unit_approve_weapon_swap(int unit_index, int new_weapon_index);
extern int unit_inventory_get_weapon(int unit_index, int16_t index);
extern int16_t unit_get_weapon_count(int unit_index);
extern void player_set_action_result(int player_index, int16_t action_result, int action_object_index, int16_t action_seat_index);

void player_examine_nearby_item_client(int player_index, int item_index)
{
    int unit = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->unit_index;
    item_datum *item_object = (item_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, item_index)->datum;
    unit_datum *unit_object = (unit_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit)->datum;

    if (item_object->object.parent_object_index != -1 || item_object->item.ignore_object_index == unit)
        return;

    item_datum *equipment = object_try_and_get_and_verify_type(item_index, object_mask_equipment);
    if (equipment)
    {
        equipment_definition *equip_definition =
            TAG_GET(equipment_definition, equipment->definition_index);
        /* DEVIATION: field 0x308 (equipment.powerup_type) is compared against itself, so this never
         * fires in this build. */
        if (unit_get_current_equipment(unit) != -1
            && (unsigned __int16)equip_definition->equipment.powerup_type
               != (unsigned __int16)equip_definition->equipment.powerup_type)
        {
            player_set_action_result(player_index, _player_action_result_swap_for_powerup, item_index, -1);
        }
    }

    item_datum *weapon_obj = object_try_and_get_and_verify_type(item_index, object_mask_weapon);
    if (weapon_obj && unit_can_use_weapon(unit, item_index))
    {
        weapon_definition *weapon_def = TAG_GET(weapon_definition, weapon_obj->definition_index);
        char dual_wield = ((unit_object->unit.control_flags & (1u << _unit_control_weapon_primary_trigger_bit)) != 0
                           || (unit_object->unit.control_flags & (1u << _unit_control_weapon_secondary_trigger_bit)) != 0);

        int current_weapon = unit_inventory_get_weapon(unit, unit_object->unit.current_weapon_index);
        int weapon_count = unit_get_weapon_count(unit);

        unsigned char current_blocks_swap = 0;
        if (weapon_count >= 2 && current_weapon != -1 && (weapon_def->weapon.flags & (1u << _weapon_doesnt_count_toward_maximum_bit)) == 0)
        {
            item_datum *current_weapon_object = (item_datum *)
                DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, current_weapon)->datum;
            weapon_definition *current_weapon_definition =
                TAG_GET(weapon_definition, current_weapon_object->definition_index);
            current_blocks_swap = (current_weapon_definition->weapon.flags & (1u << _weapon_doesnt_count_toward_maximum_bit)) != 0;
        }

        if ((!dual_wield || (weapon_def->weapon.flags & (1u << _weapon_must_be_readied_bit)) == 0)
            && !unit_should_autopick_weapon(unit, item_index)
            && !current_blocks_swap
            && unit_approve_weapon_swap(unit, item_index))
        {
            item_datum *current_weapon_obj = object_try_and_get_and_verify_type(current_weapon, object_mask_weapon);
            __int16 action = _player_action_result_swap_for_weapon;
            if (weapon_count == 1 && current_weapon_obj
                && current_weapon_obj->definition_index != weapon_obj->definition_index)
                action = _player_action_result_add_weapon_to_inventory;
            player_set_action_result(player_index, action, item_index, -1);
        }
    }
}
