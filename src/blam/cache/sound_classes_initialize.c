/* sound_classes_initialize @ 0x83739850 — allocate sound-class table
 * (NUMBER_OF_SOUND_CLASSES * sizeof(sound_class_datum) = 51 * 12 = 612 bytes) */

#include "headers/sound_class_datum.h"
#include "headers/sound_class.h"
#include "headers/blam_data_globals.h"

extern void *game_state_malloc(const char *name, const char *type, int size);

void sound_classes_initialize(void)
{
    sound_class_data = game_state_malloc("sound classes", 0,
                                                              NUMBER_OF_SOUND_CLASSES * sizeof(sound_class_datum));
}
