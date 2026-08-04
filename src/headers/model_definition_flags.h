#ifndef MODEL_DEFINITION_FLAGS_H
#define MODEL_DEFINITION_FLAGS_H

/* model_definition_flags — bit indices for the model_definition flags word.
 * DB-verified: types_enum_values _120DF71ADA0C4EF9EB46576E654EBCE3 carries these named bit
 * indices verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum model_definition_flags {
	_model_definition_blend_shared_normals_bit = 0,
	_model_definition_parts_have_local_nodes_bit = 1,
	_model_definition_ignore_skinning = 2,
	NUMBER_OF_MODEL_DEFINITION_FLAGS = 3,
};

#endif /* MODEL_DEFINITION_FLAGS_H */

// model_definition_flags: DB-verified via types_enum_values _120DF71ADA0C4EF9EB46576E654EBCE3
