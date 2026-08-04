#pragma once
/* damage_effect_definition — the 'jpt!' damage effect tag body (672 bytes; layout from the
   database, fully typed per types_members). */

#include "tag_reference.h"
#include "damage_definition.h"
#include "screen_flash_definition.h"
#include "vibrate_definition.h"
#include "camera_impulse_definition.h"
#include "camera_shake_definition.h"
#include "damage_breaking_effect_definition.h"

typedef struct damage_effect_definition
{
    float             falloff_radius;           /* 0x000 */
    float             cutoff_radius;            /* 0x004 */
    float             cutoff_scale;             /* 0x008 */
    unsigned int      flags;                    /* 0x00C */
    unsigned int      area_of_effect_unused[5]; /* 0x010 */
    screen_flash_definition           screen_flash;    /* 0x024 */
    vibrate_definition                vibrate;         /* 0x05C */
    camera_impulse_definition         camera_impulse;  /* 0x098 */
    camera_shake_definition           camera_shake;    /* 0x0CC */
    tag_reference     sound;                    /* 0x114 */
    unsigned int      unused1[28];              /* 0x124 */
    damage_breaking_effect_definition breaking_effect; /* 0x194 */
    damage_definition damage;                   /* 0x1C4 (452) */
} damage_effect_definition;                     /* 672 bytes */
