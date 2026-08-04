/* DB-verified 2026-07-20 (BLOCKED-retry): values match compiled enum types_enum_values
 * _6BF7A110AA5B3C3CC0B314DD5485B2FB exactly. */
#ifndef SPATIALIZATION_MODE_H
#define SPATIALIZATION_MODE_H

/* spatialization_mode — how a sound source is placed in the mix (sound_source.spatialization_mode):
 * none = 2D/non-positional, absolute = world-positioned, relative = listener-relative.
 * DB-AUTHORITATIVE. */
enum spatialization_mode {
	_sound_spatialization_mode_none = 0,
	_sound_spatialization_mode_absolute = 1,
	_sound_spatialization_mode_relative = 2,
	NUMBER_OF_SOUND_SPATIALIZATION_MODES = 3,
};

#endif /* SPATIALIZATION_MODE_H */
