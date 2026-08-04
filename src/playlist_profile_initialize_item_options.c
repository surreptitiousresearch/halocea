/* playlist_profile_initialize_item_options @0x83695D68 — UI init callback (sibling of
 * playlist_profile_change_item_options): when the "edit playlist profile" options screen is first shown, seed
 * the three sibling text-box selections from the profile's universal game variant. The first text box shows
 * whether variant flag 0x4 is clear (index 0 = flag set, 1 = flag clear); the second shows the weapon set
 * (universal_variant.weapon_set 1..13 selects that entry, anything else selects 0); the third shows whether
 * flag 0x20 is set. Each text box is the first widget_type==2 descendant of successive sibling groups. Returns
 * 1, or 0 if there is no profile being edited. */

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

uint8_t playlist_profile_initialize_item_options(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    game_variant *edit_playlist_profile = player_ui_get_edit_playlist_profile();
    if ( !edit_playlist_profile )
        return 0;

    widget_instance *group = widget->children;

    widget_instance *flag_box = first_text_box(group);
    flag_box->parameters.text_box_parameters.string_list_index =
        (edit_playlist_profile->universal_variant.flags & (1u << _infinite_grenades_bit)) == 0;

    widget_instance *weapon_box = first_text_box(group->next);
    switch ( edit_playlist_profile->universal_variant.weapon_set )
    {
        case _game_engine_weapons_pistols:          weapon_box->parameters.text_box_parameters.string_list_index = 1;  break;
        case _game_engine_weapons_assault_rifles:   weapon_box->parameters.text_box_parameters.string_list_index = 2;  break;
        case _game_engine_weapons_plasma_weapons:   weapon_box->parameters.text_box_parameters.string_list_index = 3;  break;
        case _game_engine_weapons_sniping:          weapon_box->parameters.text_box_parameters.string_list_index = 4;  break;
        case _game_engine_weapons_no_sniping:       weapon_box->parameters.text_box_parameters.string_list_index = 5;  break;
        case _game_engine_weapons_rocket_launchers: weapon_box->parameters.text_box_parameters.string_list_index = 6;  break;
        case _game_engine_weapons_shotguns:         weapon_box->parameters.text_box_parameters.string_list_index = 7;  break;
        case _game_engine_weapons_short_range:      weapon_box->parameters.text_box_parameters.string_list_index = 8;  break;
        case _game_engine_weapons_human:            weapon_box->parameters.text_box_parameters.string_list_index = 9;  break;
        case _game_engine_weapons_covenant:         weapon_box->parameters.text_box_parameters.string_list_index = 10; break;
        case _game_engine_weapons_classic:          weapon_box->parameters.text_box_parameters.string_list_index = 11; break;
        case _game_engine_weapons_heavy:            weapon_box->parameters.text_box_parameters.string_list_index = 12; break;
        case _game_engine_weapons_no_grenades:      weapon_box->parameters.text_box_parameters.string_list_index = 13; break;
        default:                                    weapon_box->parameters.text_box_parameters.string_list_index = 0;  break; /* _game_engine_weapons_normal */
    }

    widget_instance *third_box = first_text_box(group->next->next);
    third_box->parameters.text_box_parameters.string_list_index =
        (edit_playlist_profile->universal_variant.flags & (1u << _generic_starting_equipment_bit)) != 0;

    return 1;
}
