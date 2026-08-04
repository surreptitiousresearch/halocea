#pragma once
/* effect_event_definition — one element of effect_definition.events
 * (68 bytes, DB types_members-confirmed). */

#include "tag_block.h"

typedef struct effect_event_definition
{
    unsigned int flags;               /* 0x00 */
    float        skip_fraction;       /* 0x04 */
    float        delay_lower_bound;   /* 0x08 */
    float        delay_upper_bound;   /* 0x0C */
    float        duration_lower_bound;/* 0x10 */
    float        duration_upper_bound;/* 0x14 */
    float        unused[5];           /* 0x18 */
    tag_block    parts;               /* 0x2C — effect_part_definition[] */
    tag_block    particles;           /* 0x38 — effect_particles_definition[] */
} effect_event_definition;            /* 68 bytes */
