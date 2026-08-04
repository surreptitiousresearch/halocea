#pragma once
/* animation_list_entry — one named entry in an animation_list. Layout from the database. */

typedef struct animation_list_entry
{
    char    *name; /* 0x00 */
    __int16  type; /* 0x04 */
} animation_list_entry; /* 8 bytes (padded) */
