#pragma once
/* sound_fade_mode — fade interpolation mode for a sound instance (sound_datum.fade_mode).
 * DB anonymous enum _9496AF50B8DFC93DC551B6841F2FCF52 (verbatim). */
#ifndef SOUND_FADE_MODE_H
#define SOUND_FADE_MODE_H

enum sound_fade_mode
{
    _sound_fade_mode_linear = 0,
    _sound_fade_mode_crossfade = 1,
    NUMBER_OF_SOUND_FADE_MODES = 2
};

#endif /* SOUND_FADE_MODE_H */
