#pragma once
/* structure_fog_region — one element of structure_bsp.fog_regions (40 bytes; DB-verified). */

typedef struct structure_fog_region
{
    int     unused[9];             /* 0x00 */
    __int16 fog_palette_index;     /* 0x24 */
    __int16 weather_palette_index; /* 0x26 */
} structure_fog_region;            /* 40 bytes */
