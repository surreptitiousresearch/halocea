#ifndef GAME_LOOPING_SOUND_FLAGS_H
#define GAME_LOOPING_SOUND_FLAGS_H

/* game_looping_sound_flags — bit indices for the game_looping_sound flags word.
 * DB-verified: types_enum_values _0BF8E63E633E5A48DCF1B5534F9089EF carries these named bit
 * indices verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum game_looping_sound_flags {
	_game_looping_sound_unattached_bit = 0,
	_game_looping_sound_unattached_stop_bit = 1,
	_game_looping_sound_unattached_stop_fixed_fadeout_bit = 2,
	_game_looping_sound_alternate_bit = 3,
	_game_looping_sound_scripted_bit = 4,
	NUMBER_OF_GAME_LOOPING_SOUND_FLAGS = 5,
};

#endif /* GAME_LOOPING_SOUND_FLAGS_H */

// game_looping_sound_flags: DB-verified via types_enum_values _0BF8E63E633E5A48DCF1B5534F9089EF
