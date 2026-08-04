/* player_profile_update_item @0x8377F218 — refresh one player-profile list row's widgets from a profile (or
 * blank it when the slot is empty). With a profile: shows the populated sub-widgets, sets the name text (a
 * default-name string for built-in profiles, else the stored player_name, clamped to 11 chars), sets the color
 * swatch frame to the clamped primary color, and — for user profiles — fills the campaign progress spinners
 * (highest completed solo level, capped at 9, its difficulty) and the invert-look flag; built-in profiles hide
 * the campaign spinners instead. With no profile: hides everything and shows the empty-slot prompt.
 *
 * The child widget chain is a fixed layout; the label/value roles below are inferred from usage (label widgets
 * only toggle visibility, value widgets carry the spinner string indices). */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/player_profile.h"
#include "headers/button_preset_flags.h"

extern void * ui_widget_realloc(void *ptr, uint16_t size);
extern int tag_loaded(uint32_t group_tag, const char *name);
extern unsigned short *unicode_string_list_get_string(int tag_index, int16_t string_index);
extern void ustrncpy(wchar_t *dest, const wchar_t *src, unsigned int count);
extern int player_profile_number_of_available_primary_colors(void);
extern void player_profile_get_highest_completed_solo_level(player_profile *profile, int16_t *level, int16_t *difficulty);

void player_profile_update_item(widget_instance *item, player_profile *profile)
{
    widget_instance *name_field = item->children;
    widget_instance *color_selector = name_field->next;
    widget_instance *empty_slot_prompt = color_selector->next->children;
    widget_instance *campaign_label = empty_slot_prompt->next;
    widget_instance *campaign_level_value = campaign_label->next;
    widget_instance *difficulty_label = campaign_level_value->next;
    widget_instance *difficulty_value = difficulty_label->next;
    widget_instance *invert_look_label = difficulty_value->next;
    widget_instance *invert_look_value = invert_look_label->next;

    if ( profile )
    {
        name_field->visible = 1;
        empty_slot_prompt->visible = 0;
        campaign_label->visible = 1;
        campaign_level_value->visible = 1;
        difficulty_label->visible = 1;
        difficulty_value->visible = 1;
        invert_look_label->visible = 1;
        invert_look_value->visible = 1;

        unsigned __int16 *text = ui_widget_realloc(
            name_field->parameters.text_box_parameters.text, 0x18u);
        name_field->parameters.text_box_parameters.text = text;
        if ( text )
        {
            const wchar_t *name_string;
            unsigned int flags = profile->flags;
            if ( (flags & (1u << _profile_is_default_bit)) != 0 )
            {
                /* default profiles carry their display-name index in the high byte
                 * (_default_profile_index_mask = 0xFF00, shift 8) */
                unsigned int default_name_index = flags >> 8;
                int names_tag = tag_loaded(0x75737472u, "ui\\shell\\strings\\default_player_profile_names");
                if ( names_tag == -1 )
                    name_string = L"<unknown>";
                else
                    name_string = unicode_string_list_get_string(names_tag, default_name_index);
            }
            else
            {
                name_string = profile->player_name;
            }
            ustrncpy(text, name_string, 0xBu);
            name_field->parameters.text_box_parameters.text[11] = 0;
        }

        __int16 color_frame;
        if ( (unsigned __int16)profile->primary_color_index < 0x8000u )
        {
            int primary_color_index = profile->primary_color_index;
            int maximum = player_profile_number_of_available_primary_colors() - 1;
            if ( primary_color_index <= maximum )
                color_frame = primary_color_index;
            else
                color_frame = maximum;
        }
        else
        {
            color_frame = 0;
        }
        color_selector->animation_data.current_frame_index = color_frame;

        if ( (profile->flags & (1u << _profile_is_default_bit)) != 0 )
        {
            campaign_level_value->visible = 0;
            difficulty_value->visible = 0;
        }
        else
        {
            __int16 highest_level;
            __int16 difficulty;
            player_profile_get_highest_completed_solo_level(profile, &highest_level, &difficulty);
            int level_index = highest_level + 1;
            if ( level_index > 9 )
                level_index = 9;
            campaign_level_value->parameters.text_box_parameters.string_list_index = level_index;
            difficulty_value->parameters.text_box_parameters.string_list_index = difficulty;
            invert_look_value->parameters.text_box_parameters.string_list_index =
                profile->controller_settings.invert_look == 1;
        }
    }
    else
    {
        name_field->visible = 0;
        color_selector->animation_data.current_frame_index = player_profile_number_of_available_primary_colors();
        empty_slot_prompt->visible = 1;
        campaign_label->visible = 0;
        campaign_level_value->visible = 0;
        difficulty_label->visible = 0;
        difficulty_value->visible = 0;
        invert_look_label->visible = 0;
        invert_look_value->visible = 0;
    }
}
