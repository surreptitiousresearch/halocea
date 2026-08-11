/* render_debug_sound @0x837166A0 — when debug_sound is enabled, formats a sound's tag name and two float
 * parameters (sound datum +76/+80, see sound_stop_impulse_by_source_and_definition.c for the 176-byte
 * stride) into a scratch buffer via sprintf_0 for on-screen debug display. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/sound_datum.h"
#include "headers/blam_data_globals.h"

extern char *tag_get_name(int tag_index);
extern int sprintf_0(char *string, const char *format, ...);

void render_debug_sound(int sound_index)
{
    if ( debug_sound )
    {
        sound_datum *sound = (sound_datum *)sound_data->data + sound_index;
        float value1 = sound->source.obstruction;
        float value2 = sound->source.occlusion;
        const char *name = tag_get_name(sound->definition_index);

        char buffer[520];
        sprintf_0(buffer, "%s|n%f %f", name, value1, value2);
    }
}
