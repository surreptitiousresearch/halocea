/* game_state_free_buffer @0x8371B150 — release the physically-allocated game-state backing buffer. */

#include "headers/xbox_game_state_globals.h"

extern void XPhysicalFree(void *address);

void game_state_free_buffer(void)
{
    XPhysicalFree(xbox_game_state_globals.buffer);
    xbox_game_state_globals.buffer_allocated = 0;
}
