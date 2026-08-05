#pragma once
/* shader_transparent_plasma — full tag: 40-byte _shader base + the plasma-specific body (292 bytes,
 * DB types_members-confirmed). */

#include "_shader.h"
#include "_shader_transparent_plasma.h"

typedef struct shader_transparent_plasma
{
    _shader                      shader;   /* 0x00 */
    _shader_transparent_plasma   plasma;   /* 0x28 */
} shader_transparent_plasma;               /* 0x14C = 332 bytes */
