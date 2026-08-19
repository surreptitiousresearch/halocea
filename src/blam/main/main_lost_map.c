/* main_lost_map @0x836891F8 — flag that the player lost the current map (cancels any pending save). */

#include "headers/main_globals.h"

void main_lost_map(void)
{
    main_globals.save_map = 0;
    main_globals.lost_map = 1;
}
