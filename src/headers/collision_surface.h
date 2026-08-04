#pragma once

typedef struct collision_surface
{
    int             plane_designator;        /* 0x00 — high bit = facing */
    int             first_edge_index;        /* 0x04 — index of the first edge in the boundary loop */
    unsigned __int8 flags;                   /* 0x08 */
    unsigned __int8 breakable_surface_index; /* 0x09 */
    __int16         material_index;          /* 0x0A */
} collision_surface;                         /* 12 bytes */
