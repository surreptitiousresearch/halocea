#pragma once
/* _shader_decal — decal-level shader fields (DB types_members-confirmed, 64 bytes). */

#include "tag_reference.h"

typedef struct _shader_decal
{
    unsigned __int16 flags;                       /* 0x00 */
    __int16          type;                        /* 0x02 */
    __int16          framebuffer_blend_function;  /* 0x04 */
    unsigned __int16 pad1;                         /* 0x06 */
    int              unused1[5];                   /* 0x08 */
    tag_reference    map;                          /* 0x1C */
    int              unused2[5];                   /* 0x2C */
} _shader_decal; /* 64 bytes */
