/* main_save_core_private @0x8368A690 — perform a queued engine-core save to core.bin. */

#include "headers/main_globals.h"

extern void game_state_save_core(const char *name);

void main_save_core_private(void)
{
    game_state_save_core("core.bin");
    main_globals.save_core = 0;
}
