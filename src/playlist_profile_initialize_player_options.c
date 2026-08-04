/* playlist_profile_initialize_player_options @0x836959A8 — UI callback: when the player-options rules
 * screen opens, seed each value spinner (the widget_type==2 child of each sibling group under the screen)
 * from the edit playlist profile's universal_variant. Exact inverse of
 * playlist_profile_change_player_options; sibling of the other playlist_profile_initialize_*_rules
 * callbacks. Note the group list base is `widget->children` here (the change_ callback starts from
 * widget->parent->parent->children). The eight option groups, in order: lives (0/1/3/5 → 0..3, others 0),
 * health ((int)(health*10): 5/10/15/20/30/40 → 0..5, others 0), flag bit 0x8, respawn time
 * (0/150/300/450 → 0..3, others 0), respawn time growth (same map), odd-man-out (inverted), flag bit 0x10
 * (inverted), and suicide penalty (same map as respawn). Returns 1 (or 0 if there is no profile being
 * edited). */

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
    return 0;
}

uint8_t playlist_profile_initialize_player_options(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    game_variant *edit_playlist_profile = player_ui_get_edit_playlist_profile();
    if ( !edit_playlist_profile )
        return 0;

    widget_instance *lives_group = widget->children;
    widget_instance *lives_box = first_text_box(lives_group);
    unsigned int lives = edit_playlist_profile->universal_variant.lives;
    switch ( lives <= 5 ? lives : 0 )
    {
        case 1:  lives_box->parameters.text_box_parameters.string_list_index = 1; break;
        case 3:  lives_box->parameters.text_box_parameters.string_list_index = 2; break;
        case 5:  lives_box->parameters.text_box_parameters.string_list_index = 3; break;
        default: lives_box->parameters.text_box_parameters.string_list_index = 0; break;
    }

    widget_instance *health_group = lives_group->next;
    widget_instance *health_box = first_text_box(health_group);
    int health_tenths = (int)(edit_playlist_profile->universal_variant.health * 10.0f);
    switch ( health_tenths )
    {
        case 10: health_box->parameters.text_box_parameters.string_list_index = 1; break;
        case 15: health_box->parameters.text_box_parameters.string_list_index = 2; break;
        case 20: health_box->parameters.text_box_parameters.string_list_index = 3; break;
        case 30: health_box->parameters.text_box_parameters.string_list_index = 4; break;
        case 40: health_box->parameters.text_box_parameters.string_list_index = 5; break;
        default: health_box->parameters.text_box_parameters.string_list_index = 0; break;  /* includes 5 (0.5x) */
    }

    widget_instance *flag8_group = health_group->next;
    first_text_box(flag8_group)->parameters.text_box_parameters.string_list_index =
            (edit_playlist_profile->universal_variant.flags & (1u << _no_shields_bit)) != 0;

    widget_instance *respawn_group = flag8_group->next;
    widget_instance *respawn_box = first_text_box(respawn_group);
    switch ( edit_playlist_profile->universal_variant.respawn_time )
    {
        case 150: respawn_box->parameters.text_box_parameters.string_list_index = 1; break;
        case 300: respawn_box->parameters.text_box_parameters.string_list_index = 2; break;
        case 450: respawn_box->parameters.text_box_parameters.string_list_index = 3; break;
        default:  respawn_box->parameters.text_box_parameters.string_list_index = 0; break;
    }

    widget_instance *growth_group = respawn_group->next;
    widget_instance *growth_box = first_text_box(growth_group);
    switch ( edit_playlist_profile->universal_variant.respawn_time_growth )
    {
        case 150: growth_box->parameters.text_box_parameters.string_list_index = 1; break;
        case 300: growth_box->parameters.text_box_parameters.string_list_index = 2; break;
        case 450: growth_box->parameters.text_box_parameters.string_list_index = 3; break;
        default:  growth_box->parameters.text_box_parameters.string_list_index = 0; break;
    }

    widget_instance *odd_man_group = growth_group->next;
    first_text_box(odd_man_group)->parameters.text_box_parameters.string_list_index =
            edit_playlist_profile->universal_variant.odd_man_out ? 0 : 1;

    widget_instance *flag10_group = odd_man_group->next;
    first_text_box(flag10_group)->parameters.text_box_parameters.string_list_index =
            (edit_playlist_profile->universal_variant.flags & (1u << _always_invisible_bit)) == 0;

    /* Unlike the change_ callback, the eighth group is not null-checked here. */
    widget_instance *suicide_group = flag10_group->next;
    widget_instance *suicide_box = first_text_box(suicide_group);
    switch ( edit_playlist_profile->universal_variant.suicide_penalty )
    {
        case 150: suicide_box->parameters.text_box_parameters.string_list_index = 1; break;
        case 300: suicide_box->parameters.text_box_parameters.string_list_index = 2; break;
        case 450: suicide_box->parameters.text_box_parameters.string_list_index = 3; break;
        default:  suicide_box->parameters.text_box_parameters.string_list_index = 0; break;
    }

    return 1;
}
