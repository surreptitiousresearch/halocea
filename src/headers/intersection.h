#pragma once

/* intersection — the three-way result of a volume-versus-volume cull test: the tested volume is
 * entirely outside, straddles the boundary, or lies entirely inside. bounding_rectangles_intersect
 * (@0x837C4B20) and planes_intersect_rectangle (@0x837C4BC8) both return exactly this domain, and
 * structure_visibility_build_surfaces_traverse_node/_leaf carry it as their `intersection`
 * parameter — a parent already _intersection_in skips re-testing its children, and the two tests
 * are combined with min(). Source: types_enum_values $5560A333F2C60708F0B9C45170BB2124
 * (DB-verified). */
enum intersection {
	_intersection_out = 0,
	_intersection_spanning = 1,
	_intersection_in = 2,
	NUMBER_OF_INTERSECTION_RESULTS = 3,
};

// intersection: DB-verified via types_enum_values $5560A333F2C60708F0B9C45170BB2124
