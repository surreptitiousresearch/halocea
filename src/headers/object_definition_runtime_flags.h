#ifndef OBJECT_DEFINITION_RUNTIME_FLAGS_H
#define OBJECT_DEFINITION_RUNTIME_FLAGS_H

/* object_definition_runtime_flags — bit indices for the object_definition_runtime flags word.
 * DB-verified: types_enum_values _0A3AABEBAD31D6C35B325220EE0B1104 carries these named bit
 * indices verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum object_definition_runtime_flags {
	_object_runtime_scaled_change_colors_bit = 0,
	NUMBER_OF_OBJECT_DEFINITION_RUNTIME_FLAGS = 1,
};

#endif /* OBJECT_DEFINITION_RUNTIME_FLAGS_H */

// object_definition_runtime_flags: DB-verified via types_enum_values _0A3AABEBAD31D6C35B325220EE0B1104
