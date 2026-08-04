#include "../headers/animation_playback.h"
/* animation_playback *[4]; reloc targets current_playback x3, v1_playback */
extern animation_playback current_playback;
extern animation_playback v1_playback;
animation_playback *playback_codec[4] = {
    &current_playback,
    &current_playback,
    &current_playback,
    &v1_playback,
};
