#pragma once
#include <stdint.h>
/* sound_class_definition — per-sound-class tuning (sound_classes.c). One entry per sound class
 * index; layout verbatim from the DB. */

typedef struct sound_class_definition
{
    int16_t       maximum_number_per_definition; /* 0x00 */
    int16_t       maximum_number_per_object;     /* 0x02 */
    int           preemption_time;               /* 0x04 */
    unsigned char speech;                        /* 0x08 */
    unsigned char _pad0[1]; /* db-verified padding */
    int16_t       priority;                      /* 0x0A */
    int16_t       cache_miss_mode;               /* 0x0C */
    unsigned char _pad1[2]; /* db-verified padding */
    float         reverb_damping_factor;         /* 0x10 */
    float         effect_damping_factor;         /* 0x14 */
    float         minimum_distance;              /* 0x18 */
    float         maximum_distance;              /* 0x1C */
    float         gain_lower_bound;              /* 0x20 */
    float         gain_upper_bound;              /* 0x24 */
    unsigned char disabled;                      /* 0x28 */
    unsigned char _pad29[3];                     /* 0x29 */
} sound_class_definition;                        /* 44 bytes */

extern sound_class_definition sound_classes[];
