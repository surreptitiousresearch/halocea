/* playlist_profile_globals @ 0x8446A5B8 (.data, 164 bytes)
 * DB applied_types: _playlist_profile_globals playlist_profile_globals;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 thread_input               = 0x000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
 *   +0x9C thread                     = 0x00000000
 *   +0xA0 number_of_default_profiles = 0x0000
 *   +0xA2 initialized                = 0x00
 * data-bss zero-fill; DB applied-type `_playlist_profile_globals playlist_profile_globals;`.
 * NOTE: DB types.size=164 vs ledger gap 168 (ledger overcounts; DB authoritative).
 */
#include "../headers/playlist_profile_globals.h"

_playlist_profile_globals playlist_profile_globals;
