#pragma once
/* shader_transparent_chicago_extended — full tag: 40-byte _shader base + the extended chicago body
 * (120 bytes, DB types_members-confirmed). */

#include "_shader.h"
#include "_shader_transparent_chicago_extended.h"

typedef struct shader_transparent_chicago_extended
{
    _shader                                shader;            /* 0x00 */
    _shader_transparent_chicago_extended   chicago_extended;  /* 0x28 */
} shader_transparent_chicago_extended;                        /* 0x78 = 120 bytes */
