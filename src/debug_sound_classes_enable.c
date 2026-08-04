/* debug_sound_classes_enable @0x83739AB0 — enable or disable every sound class whose name contains the given
 * substring (51 sound classes total). Disabling a class sets its `disabled` flag. */

#include <stdint.h>
#include "headers/sound_class_definition.h"
#include "headers/sound_class.h"
#include "headers/blam_data_globals.h"

extern char *strstr(const char *haystack, const char *needle);

void debug_sound_classes_enable(const char *substring, uint8_t enabled)
{
    for ( int i = 0; i < NUMBER_OF_SOUND_CLASSES; i = (__int16)(i + 1) )
    {
        const char *name = sound_class_names[i];
        if ( *name && strstr(name, substring) )
            sound_classes[i].disabled = (enabled == 0);
    }
}
