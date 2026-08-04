#pragma once
/* antenna_definition — springy-aerial tag definition (208 bytes, DB types_members-confirmed layout).
 * Fields not yet load-bearing anywhere are left as sized opaque blobs. */

#include "tag_block.h"
#include "tag_reference.h"

typedef struct antenna_definition
{
    char          attachment_marker[32];   /* 0x00 */
    tag_reference texture;                 /* 0x20 */
    tag_reference physics;                 /* 0x30 */
    int           reference_unused[20];    /* 0x40 */
    float         spring_coefficient;      /* 0x90 */
    float         falloff_pixels;          /* 0x94 */
    float         cutoff_pixels;           /* 0x98 */
    float         runtime_total_length;    /* 0x9C */
    int           unused[9];               /* 0xA0 */
    tag_block     vertices;                /* 0xC4 */
} antenna_definition;                       /* 208 bytes */
