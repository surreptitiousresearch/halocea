/* main_save_core @0x83689240 — request that the engine core (full game state) be saved next frame. */

#include "headers/main_globals.h"

void main_save_core(void)
{
    main_globals.save_core = 1;
}
