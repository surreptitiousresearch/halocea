/* playlist_profile_change_item_options @0x83694400 — UI callback: when a menu item in the "edit playlist
 * profile" options screen changes, push the three sibling text-box selections into the profile's universal
 * game variant. The first text box toggles a variant flag (bit 0x4: index 0 sets it, index 1 clears it); the
 * second selects the weapon set (its string_list_index 0..13 becomes universal_variant.weapon_set, other values
 * leave it unchanged); the third toggles another flag (bit 0x20: index 0 clears, index 1 sets). Each text box
 * is located as the first widget_type==2 descendant of successive sibling groups. Returns 1 (0 if there is no
 * profile being edited). */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/game_variant.h"
#include "headers/game_engine_weapons.h"
#include "headers/ui_widget_type.h"
#include "headers/game_engine_universal_variant_flags.h"

extern game_variant * player_ui_get_edit_playlist_profile(void);

static widget_instance *first_text_box(widget_instance *group)
{
    for ( widget_instance *child = group->children; child; child = child->next )
    {
        if ( child->widget_type == _ui_widget_type_spinner_list )
            return child;
    }
    return nullptr;
}

uint8_t playlist_profile_change_item_options(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    game_variant *edit_playlist_profile = player_ui_get_edit_playlist_profile();
    if ( !edit_playlist_profile )
        return 0;

    widget_instance *group = widget->parent->parent->children;

    widget_instance *flag_box = first_text_box(group);
    int flag_index = (uint16_t)flag_box->parameters.text_box_parameters.string_list_index;
    if ( flag_index == 0 )
        edit_playlist_profile->universal_variant.flags |= (1u << _infinite_grenades_bit);
    else if ( flag_index == 1 )
        edit_playlist_profile->universal_variant.flags &= ~(1u << _infinite_grenades_bit);

    widget_instance *weapon_box = first_text_box(group->next);
    int weapon_set_index = weapon_box->parameters.text_box_parameters.string_list_index;
    /* the UI list index doubles as the game_engine_weapons value (0..13) */
    switch ( weapon_set_index )
    {
        case _game_engine_weapons_normal:           case _game_engine_weapons_pistols:
        case _game_engine_weapons_assault_rifles:   case _game_engine_weapons_plasma_weapons:
        case _game_engine_weapons_sniping:          case _game_engine_weapons_no_sniping:
        case _game_engine_weapons_rocket_launchers: case _game_engine_weapons_shotguns:
        case _game_engine_weapons_short_range:      case _game_engine_weapons_human:
        case _game_engine_weapons_covenant:         case _game_engine_weapons_classic:
        case _game_engine_weapons_heavy:            case _game_engine_weapons_no_grenades:
            edit_playlist_profile->universal_variant.weapon_set = weapon_set_index;
            break;
        default:
            break;
    }

    widget_instance *third_box = first_text_box(group->next->next);
    int third_index = (uint16_t)third_box->parameters.text_box_parameters.string_list_index;
    if ( third_index == 0 )
        edit_playlist_profile->universal_variant.flags &= ~(1u << _generic_starting_equipment_bit);
    else if ( third_index == 1 )
        edit_playlist_profile->universal_variant.flags |= (1u << _generic_starting_equipment_bit);

    return 1;
}
