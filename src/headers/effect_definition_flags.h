#ifndef EFFECT_DEFINITION_FLAGS_H
#define EFFECT_DEFINITION_FLAGS_H

/* effect_definition_flags — bit indices for effect_definition.flags.
 * Source: reference enum _CCC144CAAB7218482B87D1734C71DD8B (DB). Values are BIT INDICES — use as (1u << _bit). */
/* layout bound to DB enum: types_enum_values _CCC144CAAB7218482B87D1734C71DD8B */
enum effect_definition_flags {
	_effect_deleted_when_inactive_bit = 0x0,
	_effect_required_for_gameplay = 0x1,
	NUMBER_OF_EFFECT_DEFINITION_FLAGS = 0x2,
	_effect_must_be_deterministic_bit = 0x2,
};

#endif /* EFFECT_DEFINITION_FLAGS_H */
