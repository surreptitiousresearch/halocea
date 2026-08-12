#pragma once

/* contents — the collision-BSP leaf contents code carried across a BSP walk (the last_contents
 * field of test_vector_data / test_pill_new_data, and the per-leaf code computed in
 * collision_bsp_test_vector_recursive @0x837E1900). The walk seeds last_contents with 0 before
 * visiting any leaf (_contents_unknown), classifies a visited leaf from its flags bit0
 * (clear -> _contents_empty, set — double-sided surfaces — -> _contents_semi_empty), and uses 3
 * for "no leaf", i.e. outside the BSP (_contents_solid); the surface tests then fire on a
 * contents TRANSITION between those codes. Source: types_enum_values
 * $861104DD7CA64649FCAEFA93DAAC4305 (DB-verified). */
enum contents {
	_contents_unknown = 0,
	_contents_empty = 1,
	_contents_semi_empty = 2,
	_contents_solid = 3,
};

// contents: DB-verified via types_enum_values $861104DD7CA64649FCAEFA93DAAC4305
