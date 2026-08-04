/* hcex_get_local_player_profile @ 0x83699358
   Returns the per-local-player UI profile slot for a local player index (0 or
   1), or NULL for any out-of-range index. The slots are the 1988-byte
   player_data entries at the head of player_ui_globals. */

#include <stddef.h>
#include "../headers/player_ui_globals.h"

extern "C" player_profile *hcex_get_local_player_profile(short local_player_index)
{
    if ( (unsigned int)local_player_index > 1 )
        return NULL;
    /* stride is the 1988-byte profile slot (player_ui_globals.player_data[i]);
       decompiler typed the result as _player_ui_globals* but the slot is a
       player_profile. */
    return &((player_profile *)&player_ui_globals)[local_player_index];
}
