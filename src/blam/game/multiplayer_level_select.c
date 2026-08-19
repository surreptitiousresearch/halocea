/* multiplayer_level_select @0x83692570 — map-list accept handler: resolve the highlighted preview
 * item to a multiplayer map name; if that map's cache file exists, set it as the current multiplayer
 * map and remember it as the last-used map (matched case-insensitively against the map list).
 * Otherwise play the negative feedback sound. Returns whether the map exists. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/multiplayer_map_s.h"
#include "headers/ui_audio_feedback_sound.h"
#include "headers/blam_data_globals.h"

extern int map_list_get_count(void);
extern int preview_list_get_list_index_for_item(int index);
extern uint8_t cache_file_map_exists(const char *name);
extern void main_set_multiplayer_map_name(const char *name);
extern void saved_game_file_remember_last_used_multiplayer_map(char *map_name);
extern void ui_play_audio_feedback_sound(int16_t audio_feedback);
extern int stricmp(const char *a, const char *b);

uint8_t multiplayer_level_select(widget_instance *wrapper_widget, event_record *event, uint8_t *widget_deleted)
{
    int map_count = map_list_get_count();
    char *name = multiplayer_maps[
        preview_list_get_list_index_for_item(
            wrapper_widget->parameters.list_parameters.selected_list_item_index)].name;

    uint8_t map_exists = cache_file_map_exists(name);
    if ( map_exists )
    {
        main_set_multiplayer_map_name(name);
        for ( int i = 0; i < map_count; ++i )
        {
            if ( !stricmp(name, multiplayer_maps[i].name) )
            {
                saved_game_file_remember_last_used_multiplayer_map(multiplayer_maps[i].name);
                break;
            }
        }
    }

    if ( !map_exists )
        ui_play_audio_feedback_sound(_ui_audio_feedback_flag_failure);

    return map_exists;
}
