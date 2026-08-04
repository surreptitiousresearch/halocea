/* main_save_map_nonsafe @0x83689158 — request an immediate (non-safe-interval) map save. */

#include "headers/main_globals.h"

void main_save_map_nonsafe(void)
{
    main_globals.save_map = 1;
    main_globals.save_map_safely = 0;
}
