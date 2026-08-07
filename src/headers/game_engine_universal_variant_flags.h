#ifndef GAME_ENGINE_UNIVERSAL_VARIANT_FLAGS_H
#define GAME_ENGINE_UNIVERSAL_VARIANT_FLAGS_H

/* game_engine_universal_variant_flags — bit indices for universal_variant.flags (int @0x04).
 * DB-verified: types_enum_values 'game_engine_universal_variant_flags' carries these names and
 * values verbatim. Note the last three carry no `_bit` suffix in the binary's own debug info —
 * spelled here exactly as the DB has them. Values are BIT INDICES — use as (1u << _name). */
enum game_engine_universal_variant_flags {
	_allow_motion_tracker_bit = 0,
	_allow_friendly_navpoints_bit = 1,
	_infinite_grenades_bit = 2,
	_no_shields_bit = 3,
	_always_invisible_bit = 4,
	_generic_starting_equipment_bit = 5,
	_motion_tracker_no_enemies = 6,
	_game_mode_classic = 7,
	_game_mode_standard = 8,
	NUMBER_OF_GAME_ENGINE_UNIVERSAL_VARIANT_FLAGS = 9,
};

#endif /* GAME_ENGINE_UNIVERSAL_VARIANT_FLAGS_H */

// game_engine_universal_variant_flags: DB-verified via types_enum_values 'game_engine_universal_variant_flags'
