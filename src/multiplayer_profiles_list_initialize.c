/* multiplayer_profiles_list_initialize @0x83692630 — populate the multiplayer game-variant chooser list widget
 * from the game variants available to the local player. Reallocates the widget's item-index array, enumerates
 * available playlist profiles into it (padding to at least 3 slots with -1), and builds the preview bank: each
 * valid profile adds a bank item labelled with its human-readable description. When a spinner precedes the
 * list, a bank category (1 or 2) is chosen from the variant's universal flags. It also pre-selects the entry
 * matching the last-used variant directory. Always returns 1 (0 if the item array realloc fails).
 *
 * bank category: for a spinner list, non-"built-in" variants (flag 0x100 clear) map to 1 when flag 0x80 is set,
 * else 2. The event/widget_deleted callback params are unused. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/game_variant.h"
#include "headers/cached_variant_profile.h"
#include "headers/game_engine_universal_variant_flags.h"
#include "headers/blam_data_globals.h"


extern void *memset(void *dest, int value, unsigned int count);
extern uint8_t has_spinner_as_first_child(widget_instance *widget);
extern void * ui_widget_realloc(void *ptr, uint16_t size);
extern void playlist_profiles_enumerate_available_to_local_player_index(int16_t local_player_index, uint16_t *number_of_profiles, int *playlist_profile_indices);
extern void preview_list_create(void);
extern uint8_t saved_game_file_retrieve_last_used_multiplayer_variant_directory(char *directory_path);
extern int saved_game_file_find_profile_index_for_directory_path(char *directory_path, int16_t saved_game_file_type);
extern uint8_t playlist_profile_get(int playlist_profile_index, game_variant *variant);
extern void preview_list_add_item_to_bank(
    int bank, const wchar_t *description, int item_index, game_variant *variant, int item_size, uint8_t selected);

uint8_t multiplayer_profiles_list_initialize(
    widget_instance *list_widget, event_record *event, uint8_t *widget_deleted)
{
    uint8_t has_spinner = has_spinner_as_first_child(list_widget);
    delete_profile_index = -1;
    memset(cached_variant_profile, -1, sizeof(cached_variant_profile));

    int *list_items = ui_widget_realloc(list_widget->parameters.list_parameters.list_items, 0x190u);
    list_widget->parameters.list_parameters.list_items = list_items;
    if ( list_items )
    {
        uint16_t count_buffer[8];
        count_buffer[0] = 100;
        int last_used_profile_index = -1;
        playlist_profiles_enumerate_available_to_local_player_index(0, count_buffer, list_items);

        uint16_t item_count = count_buffer[0];
        if ( count_buffer[0] < 3u )
        {
            unsigned int index = count_buffer[0];
            do
            {
                list_items[index] = -1;
                index = (uint16_t)(index + 1);
                item_count = ++count_buffer[0];
            }
            while ( index < 3 );
        }
        list_widget->parameters.list_parameters.number_of_items = item_count;
        preview_list_create();

        char directory[320];
        if ( saved_game_file_retrieve_last_used_multiplayer_variant_directory(directory) )
        {
            last_used_profile_index = saved_game_file_find_profile_index_for_directory_path(directory, 1);
            if ( last_used_profile_index != -1 && count_buffer[0] )
            {
                int16_t selected = 0;
                uint16_t i = 0;
                int index = 0;
                while ( list_items[index] != last_used_profile_index )
                {
                    selected = ++i;
                    index = i;
                    if ( i >= (unsigned int)count_buffer[0] )
                        goto populate_items;
                }
                list_widget->parameters.text_box_parameters.string_list_index = selected;
            }
        }

populate_items:
        if ( count_buffer[0] )
        {
            unsigned int i = 0;
            do
            {
                game_variant variant;
                if ( playlist_profile_get(list_items[i], &variant) )
                {
                    int bank = 0;
                    if ( has_spinner && (variant.universal_variant.flags & (1u << _game_mode_standard)) == 0 )
                        bank = ((variant.universal_variant.flags & (1u << _game_mode_classic)) == 0) + 1;
                    preview_list_add_item_to_bank(
                        bank, variant.human_readable_game_description, i, &variant, 152, list_items[i] == last_used_profile_index);
                }
                i = (uint16_t)(i + 1);
            }
            while ( i < count_buffer[0] );
        }
    }

    int16_t string_list_index = list_widget->parameters.text_box_parameters.string_list_index;
    list_widget->parameters.list_parameters.list_item_top_index = -1;
    list_widget->parameters.list_parameters.selected_list_item_index = string_list_index;
    return 1;
}
