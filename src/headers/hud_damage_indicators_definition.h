#pragma once
/* hud_damage_indicators_definition — the damage-indicator block of hud_globals_definition (starts at
 * hud_globals + 0x310). Four per-edge pixel offsets, the indicator bitmap reference, the single- and
 * multiplayer sprite-sequence indices, and the packed tint color. Trailing bytes up to the next named
 * hud_globals field (loading_begin_index @ 0x3D8) are kept opaque. */

#include "tag_reference.h"

typedef struct hud_damage_indicators_definition
{
    __int16       top_offset;                   /* 0x00  (hud_globals 0x310) */
    __int16       bottom_offset;                /* 0x02 */
    __int16       left_offset;                  /* 0x04 */
    __int16       right_offset;                 /* 0x06 */
    int           unused[8];                    /* 0x08 */
    tag_reference indicator_bitmap;             /* 0x28  (.index @ 0x34 -> hud_globals 0x344) */
    __int16       sequence_index;               /* 0x38 */
    __int16       multiplayer_sequence_index;   /* 0x3A */
    unsigned int  color;                        /* 0x3C */
    int           unused2[4];                   /* 0x40 */
} hud_damage_indicators_definition;             /* 0x50 (80 bytes) */
