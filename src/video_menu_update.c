/* video_menu_update @0x837852F8 — per-tick update of the video-options menu. Focuses the resolution and
 * refresh-rate spinner widgets, writes the selected resolution's name into the resolution spinner's item text,
 * fills the refresh spinner with the "%d Hz" string for the selected resolution's current refresh index
 * (clamping the index to the resolution's available count), and adjusts the gamma value from the gamma
 * spinner's activation (-1 decreases by 5 down to 1, +1 increases by 5 up to 254, with an audio bump at the
 * limits), then pushes the gamma to the rasterizer and refreshes the menu description text. Returns 1. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/video_resolution_s.h"
#include "headers/ui_widget_type.h"
#include "headers/ui_audio_feedback_sound.h"
#include "headers/blam_data_globals.h"

extern const wchar_t empty_wide_string[]; /* .rdata @0x820309EC - the shared L"" literal (def: src/data/empty_wide_string.c) */

extern void * ui_widget_realloc(void *ptr, uint16_t size);
extern void ustrncpy(wchar_t *dest, const wchar_t *src, unsigned int count);
extern void usprintf(wchar_t *string, const wchar_t *format, ...);
extern void ui_play_audio_feedback_sound(int16_t audio_feedback);
extern void rasterizer_dx9_gamma_set_value(int value);
extern void game_options_menu_update_text_desc(widget_instance *widget);

int video_menu_update(widget_instance *widget)
{
    widget_instance *children = widget->children;
    widget_instance *next = children->next;
    widget_instance *resolution_widget = children->children->next;
    widget_instance *refresh_widget = next->children->next;

    resolution_widget->parent->focused_child = resolution_widget;
    refresh_widget->parent->focused_child = refresh_widget;

    int resolution_index = resolution_widget->parameters.text_box_parameters.string_list_index;
    uint16_t *resolution_text =
        ui_widget_realloc(resolution_widget->parameters.list_parameters.item_text, 0x20u);
    resolution_widget->parameters.list_parameters.item_text = resolution_text;
    if ( resolution_text )
    {
        const wchar_t *name;
        if ( resolution_index < 0 || resolution_index >= video_resolution_count )
            name = empty_wide_string;
        else
            name = (const wchar_t *)video_resolutions[resolution_index].name;
        ustrncpy(resolution_text, name, 0xFu);
        resolution_widget->parameters.list_parameters.item_text[15] = 0;
    }

    unsigned int refresh_index = refresh_widget->parameters.text_box_parameters.string_list_index;
    refresh_widget->parameters.list_parameters.number_of_items = video_resolutions[resolution_index].numRefreshes;
    unsigned int refresh_count = video_resolutions[resolution_index].numRefreshes;
    if ( refresh_index >= refresh_count )
    {
        int16_t clamped = refresh_count - 1;
        refresh_widget->parameters.text_box_parameters.string_list_index = clamped;
        refresh_index = clamped;
    }

    uint16_t *refresh_text =
        ui_widget_realloc(refresh_widget->parameters.list_parameters.item_text, 0x20u);
    refresh_widget->parameters.list_parameters.item_text = refresh_text;
    if ( refresh_text )
    {
        usprintf(refresh_text, L"%d Hz", video_resolutions[resolution_index].refreshes[refresh_index]);
        refresh_widget->parameters.list_parameters.item_text[15] = 0;
    }

    widget_instance *gamma_widget;
    for ( gamma_widget = next->next->next->next->next->next->next->next->children;
          gamma_widget;
          gamma_widget = gamma_widget->next )
    {
        if ( gamma_widget->widget_type == _ui_widget_type_spinner_list )
            break;
    }

    int gamma;
    int spin_activated = gamma_widget->parameters.list_parameters.spin_activated;
    if ( spin_activated == -1 )
    {
        gamma = video_gamma_setting - 5;
        video_gamma_setting = gamma;
        if ( gamma < 1 )
        {
            ui_play_audio_feedback_sound(_ui_audio_feedback_flag_failure);
            gamma = 1;
            video_gamma_setting = 1;
        }
    }
    else if ( spin_activated == 1 )
    {
        gamma = video_gamma_setting + 5;
        video_gamma_setting = gamma;
        if ( gamma > 254 )
        {
            ui_play_audio_feedback_sound(_ui_audio_feedback_flag_failure);
            gamma = 254;
            video_gamma_setting = 254;
        }
    }
    else
    {
        gamma = video_gamma_setting;
    }

    rasterizer_dx9_gamma_set_value(gamma);
    game_options_menu_update_text_desc(widget);
    return 1;
}
