/* sound_class_set_gain @0x837399E4 — set the desired gain (and fade duration in ticks) of every sound
 * class whose name contains `substring`. Gain is clamped to [0,1]; ticks are floored at 0. Scans all 51
 * sound classes.
 * DEVIATION: the decompiler invented a 4th param (a4); the disassembly stores the 3rd param (ticks, r5).
 * The carry idiom (__CFADD__) is max(ticks, 0). */

#include <stdint.h>
#include "headers/sound_class_datum.h"
#include "headers/sound_class.h"
#include "headers/blam_data_globals.h"

extern char *strstr(const char *haystack, const char *needle);

void sound_class_set_gain(const char *substring, float gain, int16_t ticks)
{
    for ( int index = 0; index < NUMBER_OF_SOUND_CLASSES; ++index )
    {
        const char *name = sound_class_names[index];
        if ( *name && strstr(name, substring) )
        {
            sound_class_datum *sound_class = &sound_class_data[index];

            float desired_gain;
            if ( gain >= 0.0f )
                desired_gain = gain <= 1.0f ? gain : 1.0f;
            else
                desired_gain = 0.0f;
            sound_class->desired_gain = desired_gain;
            sound_class->ticks = ticks < 0 ? 0 : ticks;
        }
    }
}
