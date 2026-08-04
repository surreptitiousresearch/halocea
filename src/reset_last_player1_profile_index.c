#include "headers/blam_data_globals.h"
/* reset_last_player1_profile_index @0x83691930 — clear the cached "last used player 1 profile" index to none
 * (-1). The caller's index argument is ignored in this release. */


void reset_last_player1_profile_index(int index)
{
    last_player1_profile_index = -1;
}
