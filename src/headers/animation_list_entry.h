#pragma once
#include <stdint.h>
/* animation_list_entry — one named entry in an animation_list. Layout from the database. */

typedef struct animation_list_entry
{
    char    *name; /* 0x00 */
    int16_t  type; /* 0x04 */
} animation_list_entry; /* 8 bytes (padded) */
