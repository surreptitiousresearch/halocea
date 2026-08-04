#ifndef DECAL_DEFINITION_FLAGS_H
#define DECAL_DEFINITION_FLAGS_H

/* decal_definition_flags — bit indices for decal_definition.flags (unsigned short @0x0).
 * DB-verified: types_enum_values _616BBB20FB45BEBEC97EB64C2F6E1694 carries these named
 * bit indices verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum decal_definition_flags {
	_decal_definition_geometry_inherited_by_next_decal_in_chain_bit = 0x0,
	_decal_definition_color_interpolate_in_hsv_bit = 0x1,
	_decal_definition_color_interpolate_along_farthest_hue_path_bit = 0x2,
	_decal_definition_no_random_rotation_bit = 0x3,
	_decal_definition_water_effect_bit = 0x4,
	_decal_definition_SAPIEN_ONLY_snap_to_axis_bit = 0x5,
	_decal_definition_SAPIEN_ONLY_incremental_counter_bit = 0x6,
	_decal_definition_animation_loop_bit = 0x7,
	_decal_definition_preserve_aspect_bit = 0x8,
	NUMBER_OF_DECAL_DEFINITION_FLAGS = 0x9,
};

#endif /* DECAL_DEFINITION_FLAGS_H */

// decal_definition_flags: DB-verified via types_enum_values _616BBB20FB45BEBEC97EB64C2F6E1694 (anonymous compiled enum, names sans leading _)
