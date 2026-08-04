/* sound_class_get_gain @ 0x83739998 — current smoothed gain of a sound class from its runtime datum. */

#include <stdint.h>
#include "headers/sound_class_datum.h"

float sound_class_get_gain(int16_t class_index)
{
    return sound_class_data[class_index].gain;
}
