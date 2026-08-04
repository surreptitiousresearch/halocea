#pragma once
/* playlist_profile_write_internal_input — input block for the async playlist-profile write thread.
 * Layout per database (types_members playlist_profile_write_internal_input). */

#include "game_variant.h"

typedef struct playlist_profile_write_internal_input
{
    int          index;   /* 0x00 */
    game_variant variant; /* 0x04 */
} playlist_profile_write_internal_input; /* 156 bytes */
