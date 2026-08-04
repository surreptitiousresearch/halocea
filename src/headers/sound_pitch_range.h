#pragma once
/* sound_pitch_range — one pitch range block within a 'snd!' sound tag (72 bytes, DB
 * types_members-confirmed layout). */

#include "tag_block.h"

typedef struct sound_pitch_range
{
    char              name[32];                          /* 0x00 */
    float             natural_pitch;                      /* 0x20 */
    float             bend_lower_bound;                    /* 0x24 */
    float             bend_upper_bound;                     /* 0x28 */
    __int16           actual_permutation_count;             /* 0x2C */
    unsigned __int16  plenty_of_unused_space_here;           /* 0x2E */
    float             runtime_oo_natural_pitch;               /* 0x30 */
    unsigned int      runtime_permutation_flags;              /* 0x34 */
    __int16           runtime_last_permutation_index;          /* 0x38 */
    __int16           runtime_discarded_permutation_index;      /* 0x3A */
    tag_block         permutations;                              /* 0x3C */
} sound_pitch_range; /* 72 bytes */
