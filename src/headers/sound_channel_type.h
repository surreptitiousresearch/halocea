#pragma once

/* sound_channel_type — which of the four hardware sound-channel groups an index selects. It is the
 * index domain of sound_preferences.virtual_channel_counts[4] and of sound_channel_type_flags[4]
 * (src/data/sound_channel_type_flags.c), which sound_initialize and sound_reinitialize walk together
 * to stamp each new sound_channel_datum.type_flags. The image bytes confirm the naming slot for
 * slot against sound_channel_flags.h: {8, 9, 10, 14} decode as compressed / compressed|3d /
 * compressed|stereo / compressed|stereo|44k.
 * Source: types_enum_values $34A0B810D09811FF39373B9C0E1CD48D (DB-verified). */
enum sound_channel_type {
	_sound_channel_type_mono_compressed = 0,
	_sound_channel_type_mono_compressed_3d = 1,
	_sound_channel_type_stereo_compressed = 2,
	_sound_channel_type_stereo_compressed_44k = 3,
	NUMBER_OF_SOUND_CHANNEL_TYPES = 4,
};

// sound_channel_type: DB-verified via types_enum_values $34A0B810D09811FF39373B9C0E1CD48D
