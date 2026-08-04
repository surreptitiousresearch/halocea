#pragma once
/* _shader — the common 40-byte shader base block (radiosity + physics properties + type). */

#include "shader_radiosity_properties.h"
#include "shader_physics_properties.h"

typedef struct _shader
{
    shader_radiosity_properties radiosity; /* 0x00 */
    shader_physics_properties   physics;   /* 0x20 */
    __int16                     type;      /* 0x24 */
    unsigned __int16            pad;       /* 0x26 */
} _shader;                                  /* 40 bytes */
