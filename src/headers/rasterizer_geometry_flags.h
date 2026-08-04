#ifndef RASTERIZER_GEOMETRY_FLAGS_H
#define RASTERIZER_GEOMETRY_FLAGS_H

/* rasterizer_geometry_flags — bit indices for rasterizer geometry_flags (group/parameters->geometry_flags).
 * Source: reference enum _D1E1C6DE1B1CCD4B0CD1D5AFB59F3BD8 (headers_ref). Values are BIT INDICES — use as (1u << _bit). */
enum rasterizer_geometry_flags {
	_rasterizer_geometry_no_sort_bit = 0x0,
	_rasterizer_geometry_no_queue_bit = 0x1,
	_rasterizer_geometry_no_fog_bit = 0x2,
	_rasterizer_geometry_no_zbuffer_bit = 0x3,
	_rasterizer_geometry_sky_bit = 0x4,
	_rasterizer_geometry_viewspace_bit = 0x5,
	_rasterizer_geometry_atmospheric_fog_but_no_planar_fog_bit = 0x6,
	_rasterizer_geometry_first_person_bit = 0x7,
	_rasterizer_geometry_parts_define_local_nodes_bit = 0x8,
	_rasterizer_geometry_dont_skin = 0x9,
	NUMBER_OF_GEOMETRY_FLAGS = 0xA,
};

#endif /* RASTERIZER_GEOMETRY_FLAGS_H */

// rasterizer_geometry_flags: DB-verified via types_enum_values _D1E1C6DE1B1CCD4B0CD1D5AFB59F3BD8 (anonymous compiled enum, names sans leading _)
