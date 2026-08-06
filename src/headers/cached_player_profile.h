#pragma once
/* _cached_player_profile — one cached (profile_index, player_profile) pair in the 3-wide multiplayer
 * settings list's player-profile cache (1984 bytes). `cached_player_profile` is a 3-element array of
 * these, immediately followed in memory by `cached_variant_profile` — used as an incidental
 * end-of-array sentinel by pointer comparison in the scanning code (see [[cached_variant_profile]]). */

#include "player_profile.h"

typedef struct _cached_player_profile
{
    int            profile_index; /* 0x000 */
    player_profile profile;       /* 0x004 */
} _cached_player_profile; /* 1984 bytes */

#ifdef __cplusplus
extern "C" {
#endif

extern _cached_player_profile cached_player_profile[3];

#ifdef __cplusplus
}
#endif
