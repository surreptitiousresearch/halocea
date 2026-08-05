#pragma once
/* shader_transparent_meter — full tag: 40-byte _shader base + the meter-specific body
 * (DB types_members-confirmed). */

#include "_shader.h"
#include "_shader_transparent_meter.h"

typedef struct shader_transparent_meter
{
    _shader                    shader; /* 0x00 */
    _shader_transparent_meter  meter;  /* 0x28 */
} shader_transparent_meter;            /* 0x104 = 260 bytes */
