#pragma once
/* shader_transparent_glass — full tag: 40-byte _shader base + the glass-specific body
 * (DB types_members-confirmed). */

#include "_shader.h"
#include "_shader_transparent_glass.h"

typedef struct shader_transparent_glass
{
    _shader                    shader; /* 0x00 */
    _shader_transparent_glass  glass;  /* 0x28 */
} shader_transparent_glass;            /* 0x1E0 = 480 bytes */
