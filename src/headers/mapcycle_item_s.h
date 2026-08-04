#pragma once
/* mapcycle_item_s — one entry of the mapcycle_array dynamic_array (8 bytes, DB layout). */

typedef struct mapcycle_item_s
{
    char *map_name;     /* 0x0 */
    char *variant_name; /* 0x4 */
} mapcycle_item_s; /* 8 bytes */
