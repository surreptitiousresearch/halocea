/* game_time_initialize_for_new_map @0x8369A7F8 — zero the entire game-time globals block (32 bytes / 8 dwords)
 * and mark it initialized. */

#include <string.h>

#include "headers/game_time_globals_struct.h"
#include "headers/blam_data_globals.h"


void game_time_initialize_for_new_map(void)
{
    /* recovered: for(i<8) ((int*)game_time_globals)[i]=0 -> memset over the 32-byte struct */
    memset(game_time_globals, 0, sizeof(game_time_globals_struct));
    game_time_globals->initialized = 1;
}
