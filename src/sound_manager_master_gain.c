/* sound_manager_master_gain @ 0x837155D8 — combined output gain for a sound class: the class's own
 * gain scaled by the relevant global mix buses. Music uses music*nondialog*master; dialog buses
 * (scripted_other, unit_dialog) use nondialog*master; scripted dialog-to-player/to-other/
 * force-unspatialized use master only; everything else uses effects*nondialog*master. */

#include <stdint.h>
#include "headers/sound_manager_globals.h"
#include "headers/sound_class.h"

extern float sound_class_get_gain(int16_t class_index);

float sound_manager_master_gain(int16_t class_index)
{
    float gain = sound_class_get_gain(class_index);

    if ( class_index == _sound_class_scripted_dialog_to_player
      || class_index == _sound_class_scripted_dialog_to_other
      || class_index == _sound_class_scripted_dialog_force_unspatialized )
        return sound_manager_globals.master_gain * gain;

    if ( class_index == _sound_class_music )
        return ((sound_manager_globals.music_gain * sound_manager_globals.nondialog_gain)
                * sound_manager_globals.master_gain) * gain;

    if ( class_index == _sound_class_scripted_other || class_index == _sound_class_unit_dialog )
        return (sound_manager_globals.nondialog_gain * sound_manager_globals.master_gain) * gain;

    return ((sound_manager_globals.effects_gain * sound_manager_globals.nondialog_gain)
            * sound_manager_globals.master_gain) * gain;
}
