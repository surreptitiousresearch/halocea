#ifndef EFFECT_FLAGS_H
#define EFFECT_FLAGS_H

/* effect_flags — bit indices for _effect_datum.flags runtime effect state (effect->flags).
 * Source: reference enum _F67AD989CAB0BF1C5ED0A508DF979331 (headers_ref). Values are BIT INDICES — use as (1u << _bit). */
enum effect_flags {
	_effect_in_duration_bit = 0x0,
	_effect_loop_bit = 0x1,
	_effect_stopping_bit = 0x2,
	_effect_stopped_bit = 0x3,
	_effect_invisible_bit = 0x4,
	_effect_delete_on_stop_bit = 0x5,
	_effect_nonviolent_bit = 0x6,
	NUMBER_OF_EFFECT_FLAGS = 0x7,
};

#endif /* EFFECT_FLAGS_H */

// effect_flags: DB-verified via types_enum_values _F67AD989CAB0BF1C5ED0A508DF979331 (anonymous compiled enum, names sans leading _)
