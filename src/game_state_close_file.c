/* game_state_close_file @0x8371B1B4 — close the game-state save file handle and mark it closed. */

#include "headers/xbox_game_state_globals.h"

extern int CloseHandle(void *handle);

/* ret void: r3 at blr is unmodified CloseHandle residue; 0 sampled consumers. */
void game_state_close_file(void)
{
    CloseHandle(xbox_game_state_globals.handle);
    xbox_game_state_globals.file_open = 0;
}
