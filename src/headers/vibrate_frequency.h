#pragma once

/* vibrate_frequency — index into vibrate_definition.vibrate_frequencies[2], the two rumble channels
 * of one vibration impulse. vibrate_calculate and vibrate_update both walk the pair and accumulate
 * channel 0 into the left motor amplitude and channel 1 into the right. The DB names each index
 * twice — low/high (the tag-editor field spelling) and left/right (the runtime motor) — and both
 * spellings, in DB order, are kept verbatim; the lowercase `vibrate` inside
 * NUMBER_OF_vibrate_FREQUENCIES is the DB's own spelling, not a typo.
 * Source: types_enum_values $104B0C729BFB2163EE31C70F46F3C9EC (DB-verified). */
enum vibrate_frequency {
	_vibrate_frequency_low = 0,
	_vibrate_frequency_high = 1,
	NUMBER_OF_vibrate_FREQUENCIES = 2,
	_vibrate_frequency_left = 0,
	_vibrate_frequency_right = 1,
};

// vibrate_frequency: DB-verified via types_enum_values $104B0C729BFB2163EE31C70F46F3C9EC
