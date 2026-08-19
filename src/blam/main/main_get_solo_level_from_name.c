/* main_get_solo_level_from_name @0x83689528 — map a map name to its solo campaign level index (0..9) by
 * lowercasing it and matching the known scenario stubs; returns -1 if it is not a campaign level. */

#include <stdint.h>
#include <string.h>
#include "headers/single_player_progress_flags.h"
/* strncpy declared by <string.h> */
/* strlwr declared by <string.h> */
/* strstr declared by <string.h> */

int16_t main_get_solo_level_from_name(const char *name)
{
    char lower[136];
    memset(lower, 0, 128);
    strncpy(lower, name, 0x7Fu);
    lower[127] = 0;
    strlwr(lower);

    if ( strstr(lower, "a10") ) return _single_player_map_a10;
    if ( strstr(lower, "a30") ) return _single_player_map_a30;
    if ( strstr(lower, "a50") ) return _single_player_map_a50;
    if ( strstr(lower, "b30") ) return _single_player_map_b30;
    if ( strstr(lower, "b40") ) return _single_player_map_b40;
    if ( strstr(lower, "c10") ) return _single_player_map_c10;
    if ( strstr(lower, "c20") ) return _single_player_map_c20;
    if ( strstr(lower, "c40") ) return _single_player_map_c40;
    if ( strstr(lower, "d20") ) return _single_player_map_d20;
    return strstr(lower, "d40") ? _single_player_map_d40 : -1;
}
