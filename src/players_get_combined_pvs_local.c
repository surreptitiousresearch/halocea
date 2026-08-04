/* players_get_combined_pvs_local @0x836A8430 — the combined potentially-visible-set bit vector for the
 * local (split-screen) players this frame. */

#include "headers/players_globals.h"

unsigned int *players_get_combined_pvs_local(void)
{
    return players_globals->combined_pvs_local;
}
