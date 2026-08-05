#pragma once
#include <stdint.h>

typedef struct collision_surface
{
    int             plane_designator;        /* 0x00 — high bit = facing */
    int             first_edge_index;        /* 0x04 — index of the first edge in the boundary loop */
    uint8_t flags;                   /* 0x08 */
    uint8_t breakable_surface_index; /* 0x09 */
    int16_t         material_index;          /* 0x0A */
} collision_surface;                         /* 12 bytes */
