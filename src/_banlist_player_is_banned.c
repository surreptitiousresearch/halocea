/* _banlist_player_is_banned @0x83766120 — search the banned-player list for a matching player hash;
 * return 1 if found, 0 otherwise. Entries are stride 46, hash string at +13.
 *
 * DEVIATION: the decompiler inlines the hash comparison as a byte loop; it is a plain strcmp. */

#include <stdint.h>
#include "headers/dynamic_array.h"
#include <string.h>
#include "headers/blam_data_globals.h"


uint8_t _banlist_player_is_banned(const char *hash)
{
    if ( banned_players.count <= 0 )
        return 0;
    char *entry = (char *)banned_players.elements + 13;
    for ( int i = 0; ; entry += 46 )
    {
        if ( !strcmp(entry, hash) )
            return 1;
        if ( ++i >= banned_players.count )
            return 0;
    }
}
