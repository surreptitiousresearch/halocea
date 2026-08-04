/* game_state_initialize_for_new_map @0x836845D8 — reset the saved-game state header for a new map: lock the
 * game state, invalidate any saved game, and stamp the header with the scenario name, build number, player
 * count, difficulty, cache checksum, and running allocation-size checksum. Returns the cache file checksum. */

#include <stdint.h>
#include "headers/game_state_globals.h"
#include "headers/blam_data_globals.h"
extern void *memset(void *dst, int value, unsigned int n);
extern char *strcpy(char *, const char *);


extern char *tag_get_name(int16_t tag_index);
extern int16_t game_difficulty_level_get(void);
extern uint32_t cache_files_get_checksum(void);

/* attested void: sole caller (game_initialize_for_new_map) ignores r3; the returned
 * checksum is ABI residue. Kept the local to feed header->cache_file_checksum. */
void game_state_initialize_for_new_map(void)
{
    game_state_globals.locked = 1;
    game_state_globals.saved_game_valid = 0;
    game_state_globals.revert_time = -1;
    memset(game_state_globals.header, 0, sizeof(game_state_header));

    /* deviation: decompiler inlines the scenario-name copy as a byte loop into header->map_name; uses strcpy */
    strcpy(game_state_globals.header->map_name, tag_get_name(global_scenario_index));

    *(int *)game_state_globals.header->build_number = *(int *)"01.00.01.0563";
    strcpy(&game_state_globals.header->build_number[4], "0.01.0563");

    game_state_globals.header->player_count = player_spawn_count;
    game_state_globals.header->difficulty = game_difficulty_level_get();
    game_state_globals.header->cache_file_checksum = cache_files_get_checksum();
    game_state_globals.header->allocation_size_checksum = game_state_globals.allocation_size_checksum;
}
