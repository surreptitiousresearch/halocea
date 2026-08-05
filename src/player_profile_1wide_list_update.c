/* player_profile_1wide_list_update @0x83781950 — per-frame update for the single-column ("1-wide")
 * player-profile selection list. Twin of player_profile_update_item.c (which refreshes one row); this
 * routine drives the whole list: it loads the currently-selected profile into the 3-slot
 * cached_player_profile cache, and if the selection resolves to a live cached profile it fills the two
 * visible widgets — the name field (child of the list's parent) and its sibling description field. If
 * the selection does not resolve, it compacts the underlying index list (qsort, drop -1 sentinels),
 * clamps the selection index into range, and retries; an empty list blanks both widgets and returns.
 *
 * The self-correcting retry is the decompile's outer `while(1)`: the cache-hit path `break`s out to the
 * display code, every miss path falls through to the rebuild/clamp block and loops. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/player_profile.h"
#include "headers/cached_player_profile.h"
#include "headers/cached_variant_profile.h"
#include "headers/button_preset_flags.h"  /* profile.flags bits: _profile_*_bit */

extern void player_profile_update_cache_for_nwide_list(int *profile_index, int number_of_profile_indices);
extern void qsort(void *base, unsigned int count, unsigned int size,
                  int (__fastcall *compare)(const void *, const void *));
extern int list_indices_sort_proc(const void *index1, const void *index2);
extern void * ui_widget_realloc(void *ptr, uint16_t size);
extern int tag_loaded(uint32_t group_tag, const char *name);
extern unsigned short *unicode_string_list_get_string(int tag_index, int16_t string_index);
extern void ustrncpy(wchar_t *dest, const wchar_t *src, unsigned int count);
extern void usnprintf(wchar_t *string, unsigned int size, const wchar_t *format, ...);
extern int player_profile_number_of_available_primary_colors(void);
extern const wchar_t empty_wide_string; /* .short 0 — empty wide string, "tag not loaded" fallback */

void player_profile_1wide_list_update(widget_instance *list_widget)
{
    widget_instance *children;
    widget_instance *next;
    player_profile *profile;

    for ( ; ; )
    {
        children = list_widget->parent->children;
        next = children->next;

        /* selected index = list_items[selection]; v3/v36 alias the same value (v36 is the by-ref copy) */
        int list_selection = ((int *)list_widget->parameters.list_parameters.list_items)
                                 [list_widget->parameters.text_box_parameters.string_list_index];
        player_profile_update_cache_for_nwide_list(&list_selection, 1);

        profile = 0;
        if ( list_selection != -1 )
        {
            /* Scan the 3-slot cache for the selected profile. The disasm's end-of-array test is the
             * pointer sentinel (int)v5 >= (int)cached_variant_profile == &cached_player_profile[3]. */
            int slot_index = 0;
            while ( cached_player_profile[slot_index].profile_index != list_selection )
            {
                if ( ++slot_index >= 3 )
                    goto rebuild_list;
            }
            profile = &cached_player_profile[slot_index].profile;
            if ( profile ) /* address of a cache slot — always non-null; faithful test reproduced */
                break;
        }

rebuild_list:
        if ( !list_widget->parameters.list_parameters.number_of_items )
        {
            uint16_t *empty_name = ui_widget_realloc(
                list_widget->parameters.list_parameters.item_text, 4u);
            list_widget->parameters.list_parameters.item_text = empty_name;
            if ( empty_name )
                *empty_name = 0;
            children->animation_data.current_frame_index = 0;
            uint16_t *empty_desc = ui_widget_realloc(
                next->parameters.text_box_parameters.text, 4u);
            next->parameters.text_box_parameters.text = empty_desc;
            if ( empty_desc )
                *empty_desc = 0;
            return;
        }

        int old_number_of_items = list_widget->parameters.list_parameters.number_of_items;
        void *list_items = list_widget->parameters.list_parameters.list_items;
        qsort(list_items, old_number_of_items, 4u, list_indices_sort_proc);

        int valid_count = 0;
        if ( old_number_of_items > 0 )
        {
            int *entry = (int *)list_items;
            do
            {
                if ( *entry == -1 )
                    break;
                ++valid_count;
                ++entry;
            }
            while ( valid_count < old_number_of_items );
        }

        int16_t selection = list_widget->parameters.text_box_parameters.string_list_index;
        list_widget->parameters.list_parameters.number_of_items = valid_count;
        if ( selection >= 0 )
        {
            int last = valid_count - 1;
            if ( selection > last )
                selection = last;
            list_widget->parameters.text_box_parameters.string_list_index = selection;
        }
        else
        {
            list_widget->parameters.text_box_parameters.string_list_index = 0;
        }
    }

    /* Cache hit: populate the name field. */
    uint16_t *item_text = ui_widget_realloc(
        list_widget->parameters.list_parameters.item_text, 0x18u);
    list_widget->parameters.list_parameters.item_text = item_text;
    if ( item_text )
    {
        const wchar_t *name_string;
        unsigned int flags = profile->flags;
        if ( (flags & 1) != 0 )
        {
            unsigned int default_name_index = flags >> 8;
            int names_tag = tag_loaded(0x75737472u /* 'ustr' */,
                                       "ui\\shell\\strings\\default_player_profile_names");
            name_string = &empty_wide_string;
            if ( names_tag != -1 )
                name_string = unicode_string_list_get_string(names_tag, default_name_index);
            item_text = list_widget->parameters.list_parameters.item_text;
        }
        else
        {
            name_string = profile->player_name;
        }
        ustrncpy(item_text, name_string, 0xBu);
        list_widget->parameters.list_parameters.item_text[11] = 0;

        int16_t color_frame;
        if ( (uint16_t)profile->primary_color_index < 0x8000u )
        {
            int primary_color_index = profile->primary_color_index;
            if ( primary_color_index <= player_profile_number_of_available_primary_colors() - 1 )
                color_frame = primary_color_index;
            else
                color_frame = player_profile_number_of_available_primary_colors() - 1;
        }
        else
        {
            color_frame = 0;
        }
        children->animation_data.current_frame_index = color_frame;

        /* Populate the sibling description field with the controller preset descriptions. */
        uint16_t *description = ui_widget_realloc(
            next->parameters.text_box_parameters.text, 0x200u);
        next->parameters.text_box_parameters.text = description;
        if ( description )
        {
            if ( (profile->flags & (1u << _profile_is_default_bit)) != 0 )
            {
                int joystick_tag = tag_loaded(
                    0x75737472u,
                    "ui\\shell\\main_menu\\player_profiles_select\\joystick_set_defaults_descriptions");
                int button_tag = tag_loaded(
                    0x75737472u,
                    "ui\\shell\\main_menu\\player_profiles_select\\button_set_long_descriptions");
                if ( joystick_tag == -1 || button_tag == -1 )
                {
                    *next->parameters.text_box_parameters.text = 0;
                    next->parameters.text_box_parameters.text[255] = 0;
                }
                else
                {
                    wchar_t *joystick_desc = unicode_string_list_get_string(
                        joystick_tag, profile->controller_settings.joystick_preset);
                    wchar_t *button_desc = unicode_string_list_get_string(
                        button_tag, profile->controller_settings.button_preset);
                    usnprintf(next->parameters.text_box_parameters.text, 0xFFu, L"%s%hs%s",
                              joystick_desc, "\r\n", button_desc);
                    next->parameters.text_box_parameters.text[255] = 0;
                }
            }
            else
            {
                int joystick_tag = tag_loaded(
                    0x75737472u,
                    "ui\\shell\\main_menu\\player_profiles_select\\joystick_set_short_descriptions");
                int button_tag = tag_loaded(
                    0x75737472u,
                    "ui\\shell\\main_menu\\player_profiles_select\\button_set_short_descriptions");
                if ( joystick_tag != -1 && button_tag != -1 )
                {
                    wchar_t *joystick_desc = unicode_string_list_get_string(
                        joystick_tag, profile->controller_settings.joystick_preset);
                    wchar_t *button_desc = unicode_string_list_get_string(
                        button_tag, profile->controller_settings.button_preset);
                    usnprintf(next->parameters.text_box_parameters.text, 0xFFu, L"%s%hs%s",
                              joystick_desc, "\r\n", button_desc);
                }
                next->parameters.text_box_parameters.text[255] = 0;
            }
        }
    }
}
