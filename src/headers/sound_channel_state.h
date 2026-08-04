#pragma once
/* sound_channel_state — return value of channel_get_state (the hardware channel's
 * feed status: idle, actively playing, or full/needs no more data).
 * DB anonymous enum _935E7EF3E167DD19744BFBCE1E5C0760 (verbatim). */
#ifndef SOUND_CHANNEL_STATE_H
#define SOUND_CHANNEL_STATE_H

enum sound_channel_state
{
    _sound_channel_idle = 0,
    _sound_channel_playing = 1,
    _sound_channel_full = 2,
    NUMBER_OF_SOUND_CHANNEL_STATES = 3
};

#endif /* SOUND_CHANNEL_STATE_H */
