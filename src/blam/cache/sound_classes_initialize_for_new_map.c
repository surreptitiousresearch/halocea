/* sound_classes_initialize_for_new_map @0x83739888 — reset all 51 sound-class gain states to full gain with no
 * smoothing remaining. */

#include "headers/sound_class_datum.h"
#include "headers/sound_class.h"

void sound_classes_initialize_for_new_map(void)
{
    for ( int i = 0; i < NUMBER_OF_SOUND_CLASSES; ++i )
    {
        sound_class_datum *sound_class = &sound_class_data[i];
        sound_class->gain = 1.0f;
        sound_class->ticks = 0;
        sound_class->desired_gain = 1.0f;
    }
}
