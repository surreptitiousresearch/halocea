/* sound_class_datum — runtime per-sound-class gain state (one per sound class), smoothing the
 * audible gain toward a desired value over a number of ticks (12 bytes). */
#pragma once
#include <stdint.h>

typedef struct sound_class_datum
{
    float   desired_gain; // 0x00
    float   gain;         // 0x04
    int16_t ticks;        // 0x08
} sound_class_datum;

#ifdef __cplusplus
extern "C" {
#endif

extern sound_class_datum *sound_class_data;

#ifdef __cplusplus
}
#endif
