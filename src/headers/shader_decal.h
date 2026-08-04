#pragma once
/* shader_decal — composite decal shader tag (DB types_members-confirmed, 104 bytes). */

#include "shader.h"        /* _shader */
#include "_shader_decal.h" /* _shader_decal */

typedef struct shader_decal
{
    _shader       shader; /* 0x00 */
    _shader_decal decal;  /* 0x28 */
} shader_decal; /* 104 bytes */
