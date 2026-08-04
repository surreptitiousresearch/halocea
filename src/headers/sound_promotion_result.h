#pragma once
/* sound_promotion_result — outcome of sound_definition_promote()'s per-tick playback-limit throttle.
 * DB anonymous enum _AA2732E2348CC621181E44D82F1D8882 (verbatim). */
#ifndef SOUND_PROMOTION_RESULT_H
#define SOUND_PROMOTION_RESULT_H

enum sound_promotion_result
{
    _sound_promotion_dont = 0,
    _sound_promotion_do = 1,
    _sound_promotion_dont_play = 2,
    NUMBER_OF_SOUND_PROMOTION_RESULTS = 3
};

#endif /* SOUND_PROMOTION_RESULT_H */
