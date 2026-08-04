#ifndef CLIPPING_FLAGS_H
#define CLIPPING_FLAGS_H

/* clipping_flags — clipping flags.
 * DB-verified: types_enum_values _1406E11B10556656BE051844634838AC carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum clipping_flags {
	_vertex_out_left_bit = 0,
	_vertex_out_right_bit = 1,
	_vertex_out_bottom_bit = 2,
	_vertex_out_top_bit = 3,
	_vertex_out_near_bit = 4,
	_vertex_out_far_bit = 5,
	NUMBER_OF_CLIPPING_FLAGS = 6,
	_vertex_out_no_bits = 0,
	_vertex_out_all_bits = 63,
};

#endif /* CLIPPING_FLAGS_H */

// clipping_flags: DB-verified via types_enum_values _1406E11B10556656BE051844634838AC
