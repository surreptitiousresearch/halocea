/* players_get_combined_pvs @ 0x836A8440 — return the cached combined potentially-visible-set cluster
 * bit-vector (union of every local player's PVS), recomputed each tick by players_compute_combined_pvs. */

#include "headers/players_globals.h"

unsigned int *players_get_combined_pvs(void)
{
    return players_globals->combined_pvs;
}
