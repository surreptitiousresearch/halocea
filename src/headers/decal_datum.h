#pragma once
/* decal_datum — one live decal in the global decal datum array (DB types_members-confirmed, 56 bytes).
 * decal_insert allocates the slot (setting identifier/flags/cluster/layer and the prev/next chain);
 * decal_new_from_collision fills the visual fields below. */

#include "real_point3d.h"

typedef struct decal_datum
{
    __int16          identifier;                    /* 0x00 */
    unsigned __int16 flags;                         /* 0x02 */
    __int16          cluster_index;                 /* 0x04 */
    __int16          layer;                         /* 0x06 */
    real_point3d     position;                      /* 0x08 */
    int              creation_time;                 /* 0x14 */
    char             sequence_index;                /* 0x18 */
    char             unused___was_frames_remaining; /* 0x19 */
    char             sprite_index;                  /* 0x1A */
    char             bitmap_index;                  /* 0x1B */
    float            lifetime;                      /* 0x1C */
    float            decay_time;                    /* 0x20 */
    unsigned int     color;                         /* 0x24 — packed ARGB */
    unsigned __int8  intensity;                     /* 0x28 */
    unsigned __int8  pad;                           /* 0x29 */
    __int16          quad_count;                    /* 0x2A */
    int              definition_index;              /* 0x2C */
    int              prev_decal_index;              /* 0x30 */
    int              next_decal_index;              /* 0x34 */
} decal_datum;                                      /* 56 bytes */
