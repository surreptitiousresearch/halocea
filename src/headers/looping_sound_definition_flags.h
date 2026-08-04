#ifndef LOOPING_SOUND_DEFINITION_FLAGS_H
#define LOOPING_SOUND_DEFINITION_FLAGS_H

/* looping_sound_definition_flags — bit indices for looping_sound_definition.flags (unsigned int @0x0).
 * DB-verified: types_enum_values _247B2635ECF2DD77A6C5910E72537557 carries these named
 * bit indices verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum looping_sound_definition_flags {
	_looping_sound_deafening_bit = 0x0,
	_looping_sound_fake_impulse_sound_bit = 0x1,
	_looping_sound_stops_music_bit = 0x2,
	NUMBER_OF_LOOPING_SOUND_FLAGS = 0x3,
};

#endif /* LOOPING_SOUND_DEFINITION_FLAGS_H */

// looping_sound_definition_flags: DB-verified via types_enum_values _247B2635ECF2DD77A6C5910E72537557 (anonymous compiled enum, names sans leading _)
