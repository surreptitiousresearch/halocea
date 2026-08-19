/* banlist_player_is_banned @0x83766B10 — thunk: test whether the local player is banned, by name. It passes
 * the shared name buffer (empty_string) to the name-keyed _banlist_player_is_banned; the `localid` argument
 * is not used by this build's thunk. */

#include <stdint.h>

extern const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */
extern uint8_t _banlist_player_is_banned(const char *hash);

uint8_t banlist_player_is_banned(int localid)
{
    return _banlist_player_is_banned(empty_string);
}
