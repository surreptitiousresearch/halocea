/* solo_level_initialize_list_single_player @0x836984F0 — populate the campaign level-select list for a
 * single-player game (delegates to the co-op initializer when 2+ players are spawning). It rebuilds the
 * single_player_level_data[10] table from the active player-1 profile: on a profile change it re-reads the
 * persistent-storage "most recently played" marker; it then queries the highest completed level, seeds the
 * list's initial selection to the last-played level (clamped 0..9), and for every campaign level records its
 * scenario tag path and, when unlocked (all-maps cheat, previously completed on any difficulty, the level
 * just after the highest completed, or level 0), its per-difficulty completion checkmarks, adding a preview
 * bank item per level. Finally it wires the list widget to the table (10 items) and, if a valid persistent
 * marker exists, resolves its map name to an index and clamps its difficulty; a corrupted marker raises a
 * deferred error dialog once per profile. Always returns 1.
 *
 * The `while ((const char **)cursor < &demo_scenario_0)` loops the scenario_paths_0[] array until the next
 * symbol (demo_scenario_0) — the standard address-sentinel idiom, kept literal; it covers the 10 campaign
 * levels. single_player_level_data entries are 8 bytes {scenario_tag_path, level_completed[4]}. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/player_profile.h"
#include "headers/single_player_level_data.h"
#include "headers/persistant_game_data_info.h"
#include <string.h>
#include "headers/game_difficulty.h"
#include "headers/error_type.h"
#include "headers/blam_data_globals.h"


extern uint8_t solo_level_initialize_list_coop(widget_instance *list_widget, event_record *event, uint8_t *widget_deleted);
extern int tag_loaded(uint32_t group_tag, const char *name);
extern int player_ui_get_active_player_profile_index(int16_t local_player_index);
extern uint8_t game_state_test_persistent_storage(char *map_name, int16_t *difficulty, uint8_t *corrupted);
extern void player_ui_get_active_player_profile(int16_t local_player_index, player_profile *profile);
extern void player_profile_get_highest_completed_solo_level(player_profile *profile, int16_t *level, int16_t *difficulty);
extern void preview_list_create(void);
extern int16_t player_ui_get_last_single_player_level_played(int16_t local_player_index);
extern unsigned short *unicode_string_list_get_string(int tag_index, int16_t string_index);
extern void preview_list_add_item_to_bank(int bank, const wchar_t *name, int list_index, void *data,
        int data_len, uint8_t is_selected);
extern int stricmp(const char *a, const char *b);
extern void display_error_deferred(int16_t error_code, int16_t local_player_index, uint8_t modal, uint8_t pause_game_time);

uint8_t solo_level_initialize_list_single_player(widget_instance *list_widget, event_record *event, uint8_t *widget_deleted)
{
    if ( player_spawn_count >= 2 )
    {
        memset(&persistant_game_data_info, 0, sizeof(persistant_game_data_info));
        solo_level_initialize_list_coop(list_widget, event, widget_deleted);
        return 1;
    }

    int string_list_tag = tag_loaded(0x75737472u, "ui\\shell\\main_menu\\map_list_oneline");   /* 'ustr' */
    int active_player_profile_index = player_ui_get_active_player_profile_index(0);
    memset(single_player_level_data, 0, sizeof(single_player_level_data));

    if ( active_player_profile_index != last_player1_profile_index )
    {
        memset(&persistant_game_data_info, 0, sizeof(persistant_game_data_info));
        uint8_t valid = game_state_test_persistent_storage(persistant_game_data_info.map_name,
                &persistant_game_data_info.difficulty, &persistant_game_data_info.corrupted);
        last_player1_profile_index = active_player_profile_index;
        persistant_game_data_info.valid = valid;
    }

    player_profile profile;
    player_ui_get_active_player_profile(0, &profile);
    int16_t highest_completed_level;
    int16_t highest_completed_difficulty[7];
    player_profile_get_highest_completed_solo_level(&profile, &highest_completed_level,
            highest_completed_difficulty);
    preview_list_create();

    int16_t initial_selection;
    if ( player_ui_get_last_single_player_level_played(0) >= 0 )
    {
        if ( player_ui_get_last_single_player_level_played(0) <= 9 )
            initial_selection = player_ui_get_last_single_player_level_played(0);
        else
            initial_selection = 9;
    }
    else
    {
        initial_selection = 0;
    }

    list_widget->parameters.text_box_parameters.string_list_index = initial_selection;

    const char **scenario_path = scenario_paths_0;
    int level_index = 0;
    do
    {
        single_player_level_data[level_index].scenario_tag_path = (char *)*scenario_path;
        if ( hcex_is_unlock_all_maps || profile.single_player_map_flags[level_index]
          || level_index == highest_completed_level + 1 || !level_index )
        {
            char flags = profile.single_player_map_flags[level_index];
            single_player_level_data[level_index].level_completed[0] = 1;
            single_player_level_data[level_index].level_completed[1] = (flags & 2) != 0;
            single_player_level_data[level_index].level_completed[2] = (flags & 4) != 0;
            single_player_level_data[level_index].level_completed[3] = (flags & 8) != 0;
        }

        int is_selected = list_widget->parameters.text_box_parameters.string_list_index - level_index;
        uint16_t *name = unicode_string_list_get_string(string_list_tag, level_index);
        preview_list_add_item_to_bank(0, (const wchar_t *)name, level_index, nullptr, 0, is_selected == 0);
        ++scenario_path;
        ++level_index;
    }
    while ( scenario_path < (const char **)&demo_scenario_0 );

    int16_t selection = list_widget->parameters.text_box_parameters.string_list_index;
    list_widget->parameters.list_parameters.list_items = single_player_level_data;
    list_widget->parameters.list_parameters.number_of_items = 10;
    list_widget->parameters.list_parameters.list_item_top_index = -1;
    list_widget->parameters.list_parameters.selected_list_item_index = selection;

    if ( persistant_game_data_info.valid == 1 )
    {
        const char **cursor = (const char **)scenario_paths_0;
        persistant_game_data_info.map_name[255] = 0;
        int map_index = 0;
        while ( stricmp(persistant_game_data_info.map_name, *cursor) )
        {
            ++cursor;
            ++map_index;
            if ( cursor >= (const char **)&demo_scenario_0 )
                goto resolved;
        }
        persistant_game_data_info.map_index = map_index;
        int16_t difficulty = persistant_game_data_info.difficulty;
        if ( difficulty >= game_difficulty_level_easy )
        {
            if ( difficulty > game_difficulty_level_impossible )
                difficulty = game_difficulty_level_impossible;
        }
        else
        {
            difficulty = game_difficulty_level_easy;
        }
        persistant_game_data_info.difficulty = difficulty;
resolved:
        if ( map_index == 10 )
        {
            persistant_game_data_info.valid = 0;
            return 1;
        }
    }
    else if ( persistant_game_data_info.corrupted == 1 )
    {
        int profile_index = player_ui_get_active_player_profile_index(0);
        if ( profile_index != -1 )
        {
            if ( previous_corrupt_player1_profile_index == -1 )
            {
                display_error_deferred(_error_saved_game_state_corrupt, -1, 1, 0);
                previous_corrupt_player1_profile_index = profile_index;
                return 1;
            }
            previous_corrupt_player1_profile_index = -1;
        }
    }

    return 1;
}
