#pragma once

/* debug_key_modifier — value of debug_key.modifier (+0x06), the modifier combination a bound
 * debug key requires. debug_keys_update() builds a 4-entry combo table indexed by exactly this
 * field: [0] = neither, [1] = _key_shift only, [2] = _key_control only, [3] = both — which is
 * the domain below, member for member. The struct itself lives in src/headers/debug_key.h;
 * this header carries only the field's enumerated domain.
 * Source: types_enum_values $A80D86E8125E159D14680F54C4FBA89E (DB-verified). */
enum debug_key_modifier {
	_debug_key_no_modifier = 0,
	_debug_key_shift = 1,
	_debug_key_ctrl = 2,
	_debug_key_shift_ctrl = 3,
	NUMBER_OF_DEBUG_KEY_MODIFIERS = 4,
};

// debug_key_modifier: DB-verified via types_enum_values $A80D86E8125E159D14680F54C4FBA89E
