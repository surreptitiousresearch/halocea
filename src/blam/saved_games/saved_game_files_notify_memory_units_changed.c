/* saved_game_files_notify_memory_units_changed @0x83726B68 — flag that the set of attached memory units has
 * changed so the saved-game-file layer re-enumerates on next access. */

#include "headers/saved_game_files_globals.h"

void saved_game_files_notify_memory_units_changed(void)
{
    saved_game_files_globals.memory_units_dirty = 1;
}
