/* solo_level_initialize_list_coop @0x83691940 — populate the campaign level-select list for a co-op game
 * (the 2+ player path solo_level_initialize_list_single_player.c delegates to). Rebuilds the
 * single_player_level_data[10] table from BOTH local players' profiles: a level is unlocked/completed when
 * either player has completed it, has its map flag set, or it is the level immediately after either player's
 * highest completed level (and level 0 is always unlocked). Per-difficulty completion checkmarks are the OR of
 * the two players' difficulty bits. Finally it wires the list widget to the table (10 items) and seeds the
 * initial selection from the last single-player level played (clamped 0..9). Always returns 1. Sibling of
 * solo_level_initialize_list_single_player.c (which owns single_player_level_data / scenario_paths_0).
 *
 * DEVIATION: the DB decompile renders the 10-level loop as a 2x-unrolled 5-wide loop; reproduced as a plain
 * 10-iteration loop. Its per-level "player 1 map flag" read appears in the decompile as
 * `(&profile0.single_player_map_flags[i])[1980]` — a pointer pun that lands in profile1 because sizeof
 * player_profile == 1980 and the two profiles are adjacent stack locals; written here as the honest
 * profile1.single_player_map_flags[i]. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/player_profile.h"
#include "headers/single_player_level_data.h"
#include <string.h>
#include "headers/blam_data_globals.h"


extern void player_ui_get_active_player_profile(int16_t local_player_index, player_profile *profile);
extern void player_profile_get_highest_completed_solo_level(player_profile *profile, int16_t *level, int16_t *difficulty);
extern int16_t player_ui_get_last_single_player_level_played(int16_t local_player_index);

uint8_t solo_level_initialize_list_coop(widget_instance *list_widget, event_record *event, uint8_t *widget_deleted)
{
    memset(single_player_level_data, 0, sizeof(single_player_level_data));

    player_profile profile0;
    player_ui_get_active_player_profile(0, &profile0);
    int16_t player0_highest_level;
    int16_t player0_highest_difficulty[7];   /* write-only scratch (sized per the single-player sibling) */
    player_profile_get_highest_completed_solo_level(&profile0, &player0_highest_level,
            player0_highest_difficulty);

    player_profile profile1;
    player_ui_get_active_player_profile(1, &profile1);
    int16_t player1_highest_level;
    int16_t player1_highest_difficulty[7];
    player_profile_get_highest_completed_solo_level(&profile1, &player1_highest_level,
            player1_highest_difficulty);

    for ( int level_index = 0; level_index < 10; ++level_index )
    {
        single_player_level_data[level_index].scenario_tag_path = scenario_paths_0[level_index];

        char player0_flags = profile0.single_player_map_flags[level_index];
        char player1_flags = profile1.single_player_map_flags[level_index];
        if ( player0_flags
          || level_index == player0_highest_level + 1
          || player1_flags
          || level_index == player1_highest_level + 1
          || level_index == 0 )
        {
            char combined = player1_flags | player0_flags;
            single_player_level_data[level_index].level_completed[0] = 1;
            single_player_level_data[level_index].level_completed[1] = (combined & 2) != 0;
            single_player_level_data[level_index].level_completed[2] = (combined & 4) != 0;
            single_player_level_data[level_index].level_completed[3] = (combined & 8) != 0;
        }
    }

    list_widget->parameters.list_parameters.list_items = single_player_level_data;
    list_widget->parameters.list_parameters.number_of_items = 10;

    if ( player_ui_get_last_single_player_level_played(0) >= 0 )
    {
        if ( player_ui_get_last_single_player_level_played(0) <= 9 )
            list_widget->parameters.text_box_parameters.string_list_index =
                    player_ui_get_last_single_player_level_played(0);
        else
            list_widget->parameters.text_box_parameters.string_list_index = 9;
    }
    else
    {
        list_widget->parameters.text_box_parameters.string_list_index = 0;
    }

    return 1;
}
