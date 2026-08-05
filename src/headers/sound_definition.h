#pragma once
/* sound_definition — the 'snd!' sound tag definition (sound.c). Layout verbatim from the DB. */

#include <stdint.h>
#include "tag_reference.h"
#include "tag_block.h"
#include "sound_scale_modifiers.h"

typedef struct sound_definition
{
    int                   flags;                         /* 0x00 */
    int16_t               class_index;                   /* 0x04 */
    int16_t               sample_rate;                   /* 0x06 */
    float                 minimum_distance;              /* 0x08 */
    float                 maximum_distance;              /* 0x0C */
    float                 skip_fraction;                 /* 0x10 */
    float                 pitch_lower_bound;             /* 0x14 */
    float                 pitch_upper_bound;             /* 0x18 */
    float                 inner_cone_angle;              /* 0x1C */
    float                 outer_cone_angle;              /* 0x20 */
    float                 outer_cone_gain;               /* 0x24 */
    float                 gain;                          /* 0x28 */
    float                 maximum_bend;                  /* 0x2C */
    int                   unused[3];                     /* 0x30 */
    sound_scale_modifiers scale_lower_bound;             /* 0x3C */
    sound_scale_modifiers scale_upper_bound;             /* 0x54 */
    int16_t               encoding;                      /* 0x6C */
    int16_t               compression;                   /* 0x6E */
    tag_reference         promotion_sound;               /* 0x70 */
    int16_t               promotion_count;               /* 0x80 */
    uint16_t      pad2;                          /* 0x82 */
    int                   runtime_maximum_play_time;     /* 0x84 */
    int                   runtime_promotion_counter;     /* 0x88 */
    int                   runtime_promotion_time;        /* 0x8C */
    int                   runtime_scripting_time;        /* 0x90 */
    int                   runtime_scripting_sound_index; /* 0x94 */
    tag_block             pitch_ranges;                  /* 0x98 */
} sound_definition;                                      /* 164 bytes */
