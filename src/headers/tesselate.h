#pragma once

/* tesselate — value of flag_cell.tesselation, the per-cell cloth-grid tesselation code for flag objects.
 * flag_render_proper @0x8380E3A0 switches over the whole domain and pins two members semantically:
 * value 1 emits no geometry at all (_tesselate_none) and value 0 emits BOTH triangles of the quad
 * (_tesselate_both); values 2-5 each emit exactly one triangle, and the guard on that switch is
 * `(unsigned)tesselation > 5`, matching this family's cardinality exactly. flag_tesselate_region
 * @0x8380DE90 takes the code as its `tesselation` parameter and selects the diagonal orientation from
 * it (4/5 horizontal, 4/2 vertical). Source: types_enum_values $FFA4DDACEE7B299CD427841C2791436B
 * (DB-verified). The family carries no NUMBER_OF_* terminator — the range guard stays the raw 5. */
enum tesselate {
	_tesselate_both = 0,
	_tesselate_none = 1,
	_tesselate_top_left = 2,
	_tesselate_bottom_left = 3,
	_tesselate_top_right = 4,
	_tesselate_bottom_right = 5,
};

// tesselate: DB-verified via types_enum_values $FFA4DDACEE7B299CD427841C2791436B
