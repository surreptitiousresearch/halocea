#pragma once
/* _shader — the common 40-byte shader base block (radiosity + physics properties + type). */

#include <stdint.h>
#include "shader_radiosity_properties.h"
#include "shader_physics_properties.h"

typedef struct _shader
{
    shader_radiosity_properties radiosity; /* 0x00 */
    shader_physics_properties   physics;   /* 0x20 */
    int16_t                     type;      /* 0x24 */
    uint16_t            pad;       /* 0x26 */
} _shader;                                  /* 40 bytes */
