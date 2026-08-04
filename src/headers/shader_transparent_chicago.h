#pragma once
/* shader_transparent_chicago — full tag: 40-byte _shader base + the chicago-specific body (108 bytes,
 * DB types_members-confirmed). */

#include "_shader.h"
#include "_shader_transparent_chicago.h"

typedef struct shader_transparent_chicago
{
    _shader                       shader;    /* 0x00 */
    _shader_transparent_chicago   chicago;   /* 0x28 */
} shader_transparent_chicago;                /* 0x6C = 108 bytes */
