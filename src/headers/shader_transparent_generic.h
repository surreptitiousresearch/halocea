#pragma once
/* shader_transparent_generic — full tag: 40-byte _shader base + the generic-specific body
 * (DB types_members-confirmed). */

#include "_shader.h"
#include "_shader_transparent_generic.h"

typedef struct shader_transparent_generic
{
    _shader                        shader;   /* 0x00 */
    _shader_transparent_generic    generic;  /* 0x28 */
} shader_transparent_generic;                /* 0x6C = 108 bytes */
