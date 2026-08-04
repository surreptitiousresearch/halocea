/* banlist_add_player @0x837661B4 — add a player's name/hash to the banned-players list, rejecting an
 * already-banned hash (see _banlist_player_is_banned.c). Mirrors banlist_init.c's own entry-write pattern
 * (banned_player_s: 13-byte name + 33-byte hash). */

#include <stdint.h>
#include "headers/dynamic_array.h"
#include "headers/banned_player_s.h"
#include "headers/blam_data_globals.h"

extern uint8_t _banlist_player_is_banned(const char *hash);
extern char *strncpy(char *destination, const char *source, unsigned int count);

unsigned __int8 banlist_add_player(const char *name, const char *hash)
{
    if ( _banlist_player_is_banned(hash) )
        return 0;

    int index = dynamic_array_add_element(&banned_players);
    if ( index == -1 )
        return 0;

    banned_player_s *entry = &((banned_player_s *)banned_players.elements)[index];
    strncpy(entry->name, name, 0xC);
    entry->name[12] = 0;
    strncpy(entry->hash, hash, 0x20);
    entry->hash[32] = 0;

    return 1;
}
