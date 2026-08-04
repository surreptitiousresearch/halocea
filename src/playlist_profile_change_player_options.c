/* playlist_profile_change_player_options @0x83694048 — UI callback: when the player-options rules screen is
 * accepted, read each value spinner (the widget_type==2 child of each sibling group under the screen) and
 * push its selection into the edit playlist profile's universal_variant. Sibling of the other
 * playlist_profile_change_*_rules callbacks. The eight option groups, in order: lives, health, a flag bit
 * (0x8), respawn time, respawn time growth, odd-man-out, another flag bit (0x10, inverted), and — if the
 * eighth group is present — suicide penalty. Returns 1 (or 0 if there is no profile being edited). */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/game_variant.h"
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

uint8_t playlist_profile_change_player_options(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    game_variant *edit_playlist_profile = player_ui_get_edit_playlist_profile();
    if ( !edit_playlist_profile )
        return 0;

    widget_instance *lives_group = widget->parent->parent->children;
    unsigned int lives_index = (unsigned __int16)first_text_box(lives_group)->parameters.text_box_parameters.string_list_index;
    if ( lives_index <= 3 )
    {
        int lives;
        switch ( lives_index )
        {
            case 0:  lives = 0; break;
            case 1:  lives = 1; break;
            case 2:  lives = 3; break;
            default: lives = 5; break;
        }
        edit_playlist_profile->universal_variant.lives = lives;
    }

    widget_instance *health_group = lives_group->next;
    unsigned int health_index = (unsigned __int16)first_text_box(health_group)->parameters.text_box_parameters.string_list_index;
    if ( health_index <= 5 )
    {
        float health;
        switch ( health_index )
        {
            case 1:  health = 1.0f; break;
            case 2:  health = 1.5f; break;
            case 3:  health = 2.0f; break;
            case 4:  health = 3.0f; break;
            default: health = health_index ? 4.0f : 0.5f; break;
        }
        edit_playlist_profile->universal_variant.health = health;
    }

    widget_instance *flag8_group = health_group->next;
    unsigned int flag8_index = (unsigned __int16)first_text_box(flag8_group)->parameters.text_box_parameters.string_list_index;
    if ( flag8_index == 0 )
        edit_playlist_profile->universal_variant.flags &= ~(1u << _no_shields_bit);
    else if ( flag8_index == 1 )
        edit_playlist_profile->universal_variant.flags |= (1u << _no_shields_bit);

    widget_instance *respawn_group = flag8_group->next;
    unsigned int respawn_index = (unsigned __int16)first_text_box(respawn_group)->parameters.text_box_parameters.string_list_index;
    if ( respawn_index <= 3 )
    {
        if ( respawn_index == 1 )
            edit_playlist_profile->universal_variant.respawn_time = 150;
        else if ( respawn_index == 2 )
            edit_playlist_profile->universal_variant.respawn_time = 300;
        else if ( respawn_index )
            edit_playlist_profile->universal_variant.respawn_time = 450;
        else
            edit_playlist_profile->universal_variant.respawn_time = 0;
    }

    widget_instance *growth_group = respawn_group->next;
    unsigned int growth_index = (unsigned __int16)first_text_box(growth_group)->parameters.text_box_parameters.string_list_index;
    if ( growth_index <= 3 )
    {
        if ( growth_index == 1 )
            edit_playlist_profile->universal_variant.respawn_time_growth = 150;
        else if ( growth_index == 2 )
            edit_playlist_profile->universal_variant.respawn_time_growth = 300;
        else if ( growth_index )
            edit_playlist_profile->universal_variant.respawn_time_growth = 450;
        else
            edit_playlist_profile->universal_variant.respawn_time_growth = 0;
    }

    widget_instance *odd_man_group = growth_group->next;
    unsigned int odd_man_index = (unsigned __int16)first_text_box(odd_man_group)->parameters.text_box_parameters.string_list_index;
    if ( odd_man_index == 0 )
        edit_playlist_profile->universal_variant.odd_man_out = 1;
    else if ( odd_man_index == 1 )
        edit_playlist_profile->universal_variant.odd_man_out = 0;

    widget_instance *flag10_group = odd_man_group->next;
    unsigned int flag10_index = (unsigned __int16)first_text_box(flag10_group)->parameters.text_box_parameters.string_list_index;
    if ( flag10_index == 0 )
        edit_playlist_profile->universal_variant.flags |= (1u << _always_invisible_bit);
    else if ( flag10_index == 1 )
        edit_playlist_profile->universal_variant.flags &= ~(1u << _always_invisible_bit);

    widget_instance *suicide_group = flag10_group->next;
    if ( suicide_group )
    {
        unsigned int suicide_index = (unsigned __int16)first_text_box(suicide_group)->parameters.text_box_parameters.string_list_index;
        if ( suicide_index <= 3 )
        {
            if ( suicide_index == 1 )
                edit_playlist_profile->universal_variant.suicide_penalty = 150;
            else if ( suicide_index == 2 )
                edit_playlist_profile->universal_variant.suicide_penalty = 300;
            else if ( suicide_index )
                edit_playlist_profile->universal_variant.suicide_penalty = 450;
            else
                edit_playlist_profile->universal_variant.suicide_penalty = 0;
        }
    }

    return 1;
}
