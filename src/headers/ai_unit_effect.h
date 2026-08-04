#ifndef AI_UNIT_EFFECT_H
#define AI_UNIT_EFFECT_H

/* ai_unit_effect — the kind of noise/effect a unit made that a listening actor senses
 * (actor_handle_unit_effect effect_type, latched into prop_datum.unit_effect).
 * DB-verified via types_enum_values _8DA0EB49DCDE2FBC8AFDC08208E65C02. */
enum ai_unit_effect {
	_ai_unit_effect_bump = 0,
	_ai_unit_effect_shooting = 1,
	_ai_unit_effect_death_scream = 2,
	_ai_unit_effect_magic_sight = 3,
	NUMBER_OF_AI_UNIT_EFFECTS = 4,
};

#endif /* AI_UNIT_EFFECT_H */
