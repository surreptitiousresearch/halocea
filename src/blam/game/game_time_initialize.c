#include "headers/blam_data_globals.h"
/* game_time_initialize @ 0x8369A7A8 — allocate+zero game-time globals (32 bytes) */

typedef struct game_time_globals_struct game_time_globals_struct;
extern void *game_state_malloc(const char *name, const char *type, int size);

void game_time_initialize(void)
{
    int *p;
    int i;

    game_time_globals = game_state_malloc("game time globals", 0, 32);
    /* decompiler rendered this as an 8-word zeroing loop over the 32-byte struct */
    p = (int *)game_time_globals;
    for ( i = 0; i < 8; ++i )
        p[i] = 0;
}
