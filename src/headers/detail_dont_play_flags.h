#ifndef DETAIL_DONT_PLAY_FLAGS_H
#define DETAIL_DONT_PLAY_FLAGS_H

/* detail_dont_play_flags - bit indices for the detail_dont_play.flags word.
 * DB-verified: types_enum_values _B6F40AD45F252C130F48EF24766F0529 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum detail_dont_play_flags {
	_detail_dont_play_with_alternate_bit = 0,
	_detail_dont_play_without_alternate_bit = 1,
	NUMBER_OF_LOOPING_SOUND_DETAIL_FLAGS = 2,
};

#endif /* DETAIL_DONT_PLAY_FLAGS_H */

// detail_dont_play_flags: DB-verified via types_enum_values _B6F40AD45F252C130F48EF24766F0529
