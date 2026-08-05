#pragma once
/* _shader_decal — decal-level shader fields (DB types_members-confirmed, 64 bytes). */

#include <stdint.h>
#include "tag_reference.h"

typedef struct _shader_decal
{
    uint16_t flags;                       /* 0x00 */
    int16_t          type;                        /* 0x02 */
    int16_t          framebuffer_blend_function;  /* 0x04 */
    uint16_t pad1;                         /* 0x06 */
    int              unused1[5];                   /* 0x08 */
    tag_reference    map;                          /* 0x1C */
    int              unused2[5];                   /* 0x2C */
} _shader_decal; /* 64 bytes */
