/* main_load_core_private @0x8368A658 — perform a queued engine-core load from core.bin. */

#include "headers/main_globals.h"

extern void game_state_load_core(const char *name);

void main_load_core_private(void)
{
    game_state_load_core("core.bin");
    main_globals.load_core = 0;
}
