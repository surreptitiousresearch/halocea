#pragma once
/* shader_transparent_water — full tag: 40-byte _shader base + the water-specific body
 * (DB types_members-confirmed). */

#include "_shader.h"
#include "_shader_transparent_water.h"

typedef struct shader_transparent_water
{
    _shader                    shader; /* 0x00 */
    _shader_transparent_water  water;  /* 0x28 */
} shader_transparent_water;            /* 0x140 = 320 bytes */
