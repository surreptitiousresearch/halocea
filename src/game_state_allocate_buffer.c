/* game_state_allocate_buffer @ 0x8371B0C0
   Records the requested game-state buffer in the Xbox backing-store globals,
   composes the save-game and core-save paths from the scratch disk root, and
   returns the physical base address reserved for the game state.

   Note: the returned pointer is the physical region base, NOT the buffer that
   was recorded — those are intentionally distinct. */

#include "headers/xbox_game_state_globals.h"

extern void *physical_memory_get_game_state_base_address(void);
extern char *system_get_scratch_disk_root(void);
extern int _snprintf_0(char *buffer, unsigned int count, const char *format, ...);

void *game_state_allocate_buffer(void *address, unsigned int cpu_size, unsigned int gpu_size)
{
    void *game_state_base_address = physical_memory_get_game_state_base_address();

    xbox_game_state_globals.buffer_allocated = 1;
    xbox_game_state_globals.buffer = address;
    xbox_game_state_globals.buffer_size = cpu_size + gpu_size;

    _snprintf_0(xbox_game_state_globals.saved_game_path, 0xFFu, "%s\\%s",
                system_get_scratch_disk_root(), "savegame.bin");
    _snprintf_0(xbox_game_state_globals.core_path, 0xFFu, "%s\\%s",
                system_get_scratch_disk_root(), "core_saves");

    return game_state_base_address;
}
