/* sound_classes_update @ 0x837398F0 — advance the smoothed gain of every sound class toward its
 * desired gain. Each class fades over a remaining tick count: once `ticks_elapsed` reaches that
 * count the gain snaps to the target; otherwise it moves a proportional fraction of the way and the
 * remaining count is decremented. 51 sound classes.
 *
 * Deviation: the decompiler rendered the blend fraction as a __SPAIR64__ of (ticks_elapsed, ticks);
 * that is float-division noise — the fraction is simply ticks_elapsed / ticks (the original
 * remaining count). */

#include <stdint.h>
#include "headers/sound_class_datum.h"
#include "headers/sound_class.h"

void sound_classes_update(int ticks_elapsed)
{
    int i;

    if ( ticks_elapsed <= 0 )
        return;

    for ( i = 0; i < NUMBER_OF_SOUND_CLASSES; i = (int16_t)(i + 1) )
    {
        sound_class_datum *sound_class = &sound_class_data[i];
        int16_t ticks = sound_class->ticks;

        if ( ticks <= ticks_elapsed )
        {
            sound_class->ticks = 0;
            sound_class->gain = sound_class->desired_gain;
        }
        else
        {
            float gain = sound_class->gain;
            float desired_gain = sound_class->desired_gain;
            sound_class->ticks = ticks - ticks_elapsed;
            sound_class->gain = ((float)ticks_elapsed / (float)ticks) * (desired_gain - gain) + gain;
        }
    }
}
