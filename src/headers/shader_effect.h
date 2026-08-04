#pragma once
/* shader_effect — a shader tag block extended with secondary-map/blend-effect parameters (180 bytes,
 * DB layout: base _shader followed by _shader_effect). */

#include "_shader.h"
#include "_shader_effect.h"

typedef struct shader_effect
{
    _shader        shader; /* 0x00 */
    _shader_effect effect; /* 0x28 */
} shader_effect;            /* 180 bytes */
