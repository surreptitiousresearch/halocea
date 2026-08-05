/* hud_show_action_response @0x83707FB0 — set the on-screen HUD "state message" for one player based on the
 * action the player can currently perform (or the reason a respawn failed). Reads the player's action result
 * and interaction-target object, then selects the appropriate prompt: enter/swap vehicle seat, pick up or swap
 * a weapon, pick up equipment/powerup, or a custom scripted message. Empty/one-shot weapons on the target unit
 * are skipped when scanning the inventory for a swap candidate. On any unhandled action it falls back to a
 * game-engine custom state string, or clears the state message.
 *
 * Clean decompile. Player fields use the modelled player_datum; object data uses the OBJECT_DATA salted-lookup
 * idiom (as in the sibling object files) and TAG_DEF resolves a tag-definition data pointer. Object-datum and
 * tag-definition fields past the modelled range are accessed by byte offset. Two boolean expressions the
 * decompiler emitted through comma side-effects ((v27=1,...)/(v28=0,...)) are written as the equivalent
 * direct booleans; the odd `--weapon_count; while(!weapon_count)` inventory loop is reproduced verbatim. */

#include <wchar.h>
#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/player_datum.h"
#include "headers/object_datum.h"
#include "headers/_object_definition.h"
#include "headers/weapon_definition.h"
#include "headers/unit_datum.h"
#include "headers/control_datum.h"
#include "headers/render_globals.h"
#include "headers/icon_hud_element_definition.h"
#include "headers/weapon_hud_interface_definition.h"
#include "headers/weapon_interface_state.h"
#include "headers/global_tag_instances.h"
#include "headers/player_respawn_failure.h"
#include "headers/player_action_result.h"
#include "headers/object_type.h"
#include "headers/unit_definition.h"
#include "headers/unit_seat.h"
#include "headers/unit_seat_flags.h"
#include "headers/blam_data_globals.h"


extern int16_t players_get_respawn_failure(void);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern int get_object_icon_text_index(int object_index);
extern int unit_get_current_equipment(int unit_index);
extern int unit_inventory_get_weapon(int unit_index, int16_t index);
extern int16_t unit_get_weapon_count(int unit_index);
extern int16_t unit_inventory_next_weapon(int unit_index, int16_t current_index, int16_t delta);
extern void weapon_build_weapon_interface_state(int weapon_index, weapon_interface_state *state);
extern uint8_t game_engine_get_state_message(int player_index, uint16_t *buffer, int buffer_length);
extern void hud_set_state_message(int16_t local_player_index, int16_t message_index);
extern void hud_set_state_message_text(int16_t local_player_index, int16_t custom_icon_index,
        int16_t icon_string_index, uint8_t uses_scenario_names);
extern void hud_set_state_message_icon(int16_t local_player_index, int16_t custom_icon_index, const icon_hud_element_definition *icon);
extern void hud_enable_custom_state_message(int16_t local_player_index, uint8_t enabled);
extern void hud_set_state_text(int16_t local_player_index, const wchar_t *message);

/* datum pointer for object `idx` from the object header array (salted table index, +2 = data pointer). */
/* data pointer of the loaded tag whose index is `defidx`. */
#define TAG_DEF(defidx) (TAG_GET(char, defidx))

void hud_show_action_response(int player_index)
{
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
    int16_t respawn_failure = players_get_respawn_failure();

    if (respawn_failure && player->unit_index == -1)
    {
        switch (respawn_failure)
        {
            case _player_respawn_failure_enemies: hud_set_state_message(render.local_player_index, 10); break;
            case _player_respawn_failure_moving:  hud_set_state_message(render.local_player_index, 9);  break;
            case _player_respawn_failure_combat:  hud_set_state_message(render.local_player_index, 11); break;
            default: /* _player_respawn_failure_vehicle */
                     hud_set_state_message(render.local_player_index, 12); break;
        }
        return;
    }

    int action_object = player->action_object_index;
    /* item-name string index = the interaction target's object definition icon_text_index. */
    int16_t item_name_index = (action_object == -1)
                                  ? -1
                                  : ((const _object_definition *)TAG_DEF(
                                        ((object_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, action_object)->datum)
                                            ->definition_index))->icon_text_index;

    /* action_result is a player_action_result (DB enum $8590DCD5). The hud_set_state_message indices are
     * tag-data ordinals (hud_globals state-message elements) and remain unenumerated. */
    switch (player->action_result)
    {
        case _player_action_result_pickup_powerup:
        case _player_action_result_pickup_weapon:
            hud_set_state_message(render.local_player_index, 0);
            hud_set_state_message_text(render.local_player_index, 0, item_name_index, 0);
            break;

        case _player_action_result_exit_vehicle:
        {
            unit_datum *target_datum = (unit_datum *)(DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, player->unit_index)->datum);
            hud_set_state_message(render.local_player_index, 7);
            int16_t icon_index = get_object_icon_text_index(target_datum->object.parent_object_index);
            hud_set_state_message_text(render.local_player_index, 0, icon_index, 0);
            break;
        }

        case _player_action_result_swap_for_powerup:
        {
            hud_set_state_message(render.local_player_index, 1);
            int16_t equipment_icon = get_object_icon_text_index(unit_get_current_equipment(player->unit_index));
            hud_set_state_message_text(render.local_player_index, 0, equipment_icon, 0);
            hud_set_state_message_text(render.local_player_index, 1, item_name_index, 0);
            break;
        }

        case _player_action_result_swap_for_weapon:
        {
            object_datum *target = object_try_and_get_and_verify_type(action_object, object_mask_weapon);
            if (target)
            {
                const icon_hud_element_definition *icon = nullptr;
                int icon_tag_index = ((weapon_definition *)TAG_DEF(target->definition_index))->weapon.interface_definition.hud_interface.index;
                if (icon_tag_index != -1)
                {
                    /* recovered: TAG_DEF+316 -> weapon_hud_interface_definition.messaging_icon (0x13C) */
                    const icon_hud_element_definition *messaging_icon =
                        &((const weapon_hud_interface_definition *)TAG_DEF(icon_tag_index))->messaging_icon;
                    icon = (messaging_icon->sequence_index == -1) ? nullptr : messaging_icon;
                }
                hud_set_state_message(render.local_player_index, 4);
                int16_t local_player_index = render.local_player_index;
                if (icon)
                    hud_set_state_message_icon(local_player_index, 0, icon);
                else
                    hud_set_state_message_text(local_player_index, 0, item_name_index, 0);
            }
            break;
        }

        case _player_action_result_add_weapon_to_inventory:
        {
            object_datum *target = object_try_and_get_and_verify_type(action_object, object_mask_weapon);
            if (target)
            {
                const icon_hud_element_definition *icon = nullptr;
                int icon_tag_index = ((weapon_definition *)TAG_DEF(target->definition_index))->weapon.interface_definition.hud_interface.index;
                if (icon_tag_index != -1)
                {
                    /* recovered: TAG_DEF+316 -> weapon_hud_interface_definition.messaging_icon (0x13C) */
                    const icon_hud_element_definition *messaging_icon =
                        &((const weapon_hud_interface_definition *)TAG_DEF(icon_tag_index))->messaging_icon;
                    icon = (messaging_icon->sequence_index == -1) ? nullptr : messaging_icon;
                }
                hud_set_state_message(render.local_player_index, 0);
                int16_t local_player_index = render.local_player_index;
                if (icon)
                    hud_set_state_message_icon(local_player_index, 0, icon);
                else
                    hud_set_state_message_text(local_player_index, 0, item_name_index, 0);
            }
            break;
        }

        case _player_action_result_enter_vehicle:
        case _player_action_result_evict_from_vehicle:
        {
            /* vehicle seat label = unit_definition->unit.seats[seat].icon_text_index (DB). */
            hud_set_state_message(render.local_player_index, 6);
            object_datum *vehicle_object = DATA_ARRAY_ELEMENT(
                object_header_data, object_header_datum, action_object)->datum;
            unit_definition *vehicle_def = (unit_definition *)TAG_DEF(vehicle_object->definition_index);
            int16_t seat_label = ((unit_seat *)vehicle_def->unit.seats.address)[player->action_seat_index].icon_text_index;
            hud_set_state_message_text(render.local_player_index, 0, seat_label, 0);
            hud_set_state_message_text(render.local_player_index, 1, item_name_index, 0);
            break;
        }

        case _player_action_result_touch_device:
        {
            control_datum *target_datum = (control_datum *)(DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, action_object)->datum);
            int16_t label = target_datum->control.hud_override_index;
            if ((uint16_t)label == 0xFFFF)
            {
                hud_set_state_message(render.local_player_index, 2);
                hud_set_state_message_text(render.local_player_index, 0, item_name_index, 0);
            }
            else
            {
                hud_set_state_message(render.local_player_index, 3);
                hud_set_state_message_text(render.local_player_index, 0, label, 1);
            }
            break;
        }

        case _player_action_result_flip_vehicle:
        {
            hud_set_state_message(render.local_player_index, 8);
            int16_t icon_index = get_object_icon_text_index(action_object);
            hud_set_state_message_text(render.local_player_index, 0, icon_index, 0);
            break;
        }

        default:
        {
            wchar_t state_message[1028];
            if (game_engine_get_state_message(player_index, state_message, 1024))
            {
                hud_enable_custom_state_message(render.local_player_index, 1);
                hud_set_state_text(render.local_player_index, state_message);
            }
            else if (player->unit_index == -1)
            {
                hud_enable_custom_state_message(render.local_player_index, 0);
            }
            else
            {
                unit_datum *unit = (unit_datum *)(DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, player->unit_index)->datum);
                int weapon = unit_inventory_get_weapon(player->unit_index, unit->unit.current_weapon_index);
                char custom_shown = 0;
                char allow_swap_prompt = 1;

                if (unit->object.parent_object_index != -1)
                {
                    int16_t weapon_slot = unit->unit.parent_seat_index;
                    if (weapon_slot != -1)
                    {
                        int parent_vehicle = unit->object.parent_object_index;
                        /* Deviation: old comment called this "weapon-type flags"; DB says it is the
                         * occupied unit_seat's flags — suppress the swap prompt in driver/gunner seats. */
                        object_datum *vehicle_object = DATA_ARRAY_ELEMENT(
                            object_header_data, object_header_datum, parent_vehicle)->datum;
                        unit_definition *vehicle_def = (unit_definition *)TAG_DEF(vehicle_object->definition_index);
                        unsigned int seat_flags = ((unit_seat *)vehicle_def->unit.seats.address)[weapon_slot].flags;
                        allow_swap_prompt = (seat_flags & ((1u << _unit_seat_is_driver_bit) | (1u << _unit_seat_is_gunner_bit))) == 0;
                    }
                }

                if (weapon != -1 && allow_swap_prompt)
                {
                    weapon_interface_state weapon_state;
                    weapon_build_weapon_interface_state(weapon, &weapon_state);
                    char current_depleted = (weapon_state.magazines[0].rounds_loaded_maximum
                                             && !weapon_state.magazines[0].rounds_loaded
                                             && !weapon_state.magazines[0].rounds_remaining)
                                            || weapon_state.age == 1.0f;
                    if (current_depleted)
                    {
                        int16_t slot = unit->unit.current_weapon_index;
                        int16_t weapon_count = unit_get_weapon_count(player->unit_index);
                        int candidate = 0;
                        double candidate_age = 0.0;
                        do
                        {
                            slot = unit_inventory_next_weapon(player->unit_index, slot, 1);
                            candidate = unit_inventory_get_weapon(player->unit_index, slot);
                            weapon_build_weapon_interface_state(candidate, &weapon_state);
                            candidate_age = weapon_state.age;
                            char candidate_depleted = (weapon_state.magazines[0].rounds_loaded_maximum
                                                       && !weapon_state.magazines[0].rounds_loaded
                                                       && !weapon_state.magazines[0].rounds_remaining)
                                                      || candidate_age == 1.0f;
                            if (!candidate_depleted)
                                break;
                            if (candidate == weapon)
                                break;
                            --weapon_count;
                        } while (!weapon_count);

                        char final_depleted = (weapon_state.magazines[0].rounds_loaded_maximum
                                               && !weapon_state.magazines[0].rounds_loaded
                                               && !weapon_state.magazines[0].rounds_remaining)
                                              || candidate_age == 1.0f;
                        if (!final_depleted && candidate != weapon)
                        {
                            hud_set_state_message(render.local_player_index, 5);
                            object_datum *candidate_target = object_try_and_get_and_verify_type(candidate, object_mask_weapon);
                            if (candidate_target)
                            {
                                int icon_tag_index = ((weapon_definition *)TAG_DEF(candidate_target->definition_index))->weapon.interface_definition.hud_interface.index;
                                const icon_hud_element_definition *icon;
                                if (icon_tag_index == -1
                                    /* recovered: TAG_DEF+316 -> weapon_hud_interface_definition.messaging_icon (0x13C) */
                                    || (icon = &((const weapon_hud_interface_definition *)TAG_DEF(icon_tag_index))->messaging_icon,
                                        (uint16_t)icon->sequence_index == 0xFFFF))
                                {
                                    hud_set_state_message_text(render.local_player_index, 0, item_name_index, 0);
                                }
                                else
                                {
                                    hud_set_state_message_icon(render.local_player_index, 0, icon);
                                }
                                custom_shown = 1;
                            }
                        }
                    }
                }

                if (!custom_shown)
                    hud_enable_custom_state_message(render.local_player_index, 0);
            }
            break;
        }
    }
}
