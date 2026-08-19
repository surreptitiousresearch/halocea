#include "headers/blam_data_globals.h"
/* sound_classes_dispose @ 0x837398E0 — drop the sound-class table pointer */

typedef struct sound_class_datum sound_class_datum;

void sound_classes_dispose(void)
{
    sound_class_data = 0;
}
