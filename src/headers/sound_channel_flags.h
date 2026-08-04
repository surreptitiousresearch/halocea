#ifndef SOUND_CHANNEL_FLAGS_H
#define SOUND_CHANNEL_FLAGS_H

/* sound_channel_flags - bit indices for the sound_channel.flags word.
 * DB-verified: types_enum_values _77EB94E79DD89D53FB8987C6F5087D64 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum sound_channel_flags {
	_sound_channel_3d_bit = 0,
	_sound_channel_stereo_bit = 1,
	_sound_channel_44k_bit = 2,
	_sound_channel_compressed_bit = 3,
	NUMBER_OF_SOUND_CHANNEL_TYPE_FLAGS = 4,
};

#endif /* SOUND_CHANNEL_FLAGS_H */

// sound_channel_flags: DB-verified via types_enum_values _77EB94E79DD89D53FB8987C6F5087D64
