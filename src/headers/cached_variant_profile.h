#pragma once
/* _cached_variant_profile — one cached (profile_index, game_variant) pair in the 3-wide multiplayer
 * settings list's variant cache (156 bytes). `cached_variant_profile` is a 3-element array of these
 * (468 bytes total, DB-confirmed), immediately followed in memory by `object_list_header_data` — used as
 * an incidental end-of-array sentinel by pointer comparison in the scanning code. */

#include "game_variant.h"
#include "data_array.h"

typedef struct _cached_variant_profile
{
    int          profile_index; /* 0x00 */
    game_variant variant;       /* 0x04 */
} _cached_variant_profile; /* 156 bytes */

extern _cached_variant_profile cached_variant_profile[3];
extern data_array *object_list_header_data; /* DB applied type (was void*) */
