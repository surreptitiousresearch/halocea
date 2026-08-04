#pragma once
/* damage_region — one destructible region of a collision model's damage_resistance
   (84 bytes; layout from the database). flags bit 0x10 = shield-controlled region. */

#include "tag_reference.h"
#include "tag_block.h"

typedef struct damage_region
{
    char          name[32];          /* 0x00 */
    unsigned int  flags;             /* 0x20 */
    int           pad;               /* 0x24 */
    float         damage_threshold;  /* 0x28 */
    int           unused[3];         /* 0x2C */
    tag_reference destroyed_effect;  /* 0x38 */
    tag_block     permutations;      /* 0x48 */
} damage_region;                     /* 84 bytes */
