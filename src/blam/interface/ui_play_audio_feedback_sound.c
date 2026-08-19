/* ui_play_audio_feedback_sound @0x83733210 — plays a canned UI SFX for one of 4 feedback types (cursor move,
 * forward/back navigation, or the flag-failure buzz used as the default/out-of-range case). */

#include <stdint.h>
#include "headers/ui_audio_feedback_sound.h"

extern int tag_loaded(uint32_t group_tag, const char *name);
extern int unspatialized_impulse_sound_new(int definition_index, float scale);

void ui_play_audio_feedback_sound(int16_t audio_feedback)
{
    const char *sound_name;

    if ( audio_feedback < _ui_audio_feedback_cursor || audio_feedback > _ui_audio_feedback_flag_failure )
        return;

    switch ( audio_feedback )
    {
        case _ui_audio_feedback_forward:
            sound_name = "sound\\sfx\\ui\\forward";
            break;
        case _ui_audio_feedback_back:
            sound_name = "sound\\sfx\\ui\\back";
            break;
        case _ui_audio_feedback_cursor:
            sound_name = "sound\\sfx\\ui\\cursor";
            break;
        default: /* _ui_audio_feedback_flag_failure */
            sound_name = "sound\\sfx\\ui\\flag_failure";
            break;
    }

    int sound_tag_index = tag_loaded(0x736E6421u /* 'snd!' */, sound_name);
    if ( sound_tag_index != -1 )
        unspatialized_impulse_sound_new(sound_tag_index, 1.0f);
}
