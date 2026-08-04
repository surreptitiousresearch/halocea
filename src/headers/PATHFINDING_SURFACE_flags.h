#ifndef PATHFINDING_SURFACE_FLAGS_H
#define PATHFINDING_SURFACE_FLAGS_H

/* PATHFINDING_SURFACE_flags - bit indices for the PATHFINDING_SURFACE.flags word.
 * DB-verified: types_enum_values _A309066D78249DA00BEBF81D8A9D2805 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum PATHFINDING_SURFACE_flags {
	PATHFINDING_SURFACE_WIDTH_FIRST_BIT = 0,
	PATHFINDING_SURFACE_HEIGHT_FIRST_BIT = 3,
	PATHFINDING_SURFACE_WALKABLE_BIT = 6,
	PATHFINDING_SURFACE_BREAKABLE_BIT = 7,
};

#endif /* PATHFINDING_SURFACE_FLAGS_H */

// PATHFINDING_SURFACE_flags: DB-verified via types_enum_values _A309066D78249DA00BEBF81D8A9D2805
