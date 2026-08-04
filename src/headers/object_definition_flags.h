#ifndef OBJECT_DEFINITION_FLAGS_H
#define OBJECT_DEFINITION_FLAGS_H

/* object_definition_flags — bit indices for the shared _object_definition.flags (unsigned short @0x2).
 * DB-verified: types_enum_values _772C7EF8B3D40B72C8FB34A3FF10A4BD carries these named
 * bit indices verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum object_definition_flags {
	_object_does_not_cast_shadow_bit = 0x0,
	_object_transparency_self_occludes_bit = 0x1,
	_object_artificially_bright_bit = 0x2,
	_object_not_pathfinding_obstacle_bit = 0x3,
	NUMBER_OF_OBJECT_DEFINITION_FLAGS = 0x4,
};

#endif /* OBJECT_DEFINITION_FLAGS_H */

// object_definition_flags: DB-verified via types_enum_values _772C7EF8B3D40B72C8FB34A3FF10A4BD (anonymous compiled enum, names sans leading _)
