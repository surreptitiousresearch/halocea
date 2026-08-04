#pragma once
/* breakable_surface_datum — DB type wrapping a single per-surface vitality float (4 bytes). Byte-identical
 * to the plain `float` element type breakable_surface_globals.h already models for
 * breakable_surface_globals::breakable_surfaces[][]; kept as a distinct type only for pointer-return sites
 * like breakable_surface_get that the database types as breakable_surface_datum* rather than float*. */

typedef struct breakable_surface_datum
{
    float vitality; /* 0x00 */
} breakable_surface_datum; /* 4 bytes */
