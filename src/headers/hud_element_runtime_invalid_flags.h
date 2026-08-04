#ifndef HUD_ELEMENT_RUNTIME_INVALID_FLAGS_H
#define HUD_ELEMENT_RUNTIME_INVALID_FLAGS_H

/* hud_element_runtime_invalid_flags - bit indices for the hud_element_runtime_invalid.flags word.
 * DB-verified: types_enum_values _8B52ED9B7276D78AAC34C2C264ACFB53 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum hud_element_runtime_invalid_flags {
	_hud_element_runtime_invalid_bit = 0,
	NUMBER_OF_HUD_RUNTIME_FLAGS = 1,
};

#endif /* HUD_ELEMENT_RUNTIME_INVALID_FLAGS_H */

// hud_element_runtime_invalid_flags: DB-verified via types_enum_values _8B52ED9B7276D78AAC34C2C264ACFB53
