#pragma once
/* looping_sound_definition — the 'lsnd' looping sound tag definition (sound.c).
 * 84 bytes, DB types_members-confirmed layout. */

#include "tag_reference.h"
#include "tag_block.h"
#include "looping_sound_scale_modifiers.h"

typedef struct looping_sound_definition
{
    unsigned int                  flags;                          /* 0x00 */
    looping_sound_scale_modifiers scale_lower_bound;              /* 0x04 */
    looping_sound_scale_modifiers scale_upper_bound;              /* 0x10 */
    int                           runtime_scripting_sound_index;  /* 0x1C */
    float                         runtime_maximum_distance;       /* 0x20 */
    int                           unused[2];                      /* 0x24 */
    tag_reference                 continuous_damage_effect;       /* 0x2C */
    tag_block                     tracks;                         /* 0x3C */
    tag_block                     details;                        /* 0x48 */
} looping_sound_definition;                                       /* 84 bytes */
