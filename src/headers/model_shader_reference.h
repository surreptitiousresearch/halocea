#pragma once
/* model_shader_reference — one entry of model->shaders. Layout from the database (32 bytes). */

#include <stdint.h>
#include "tag_reference.h"

typedef struct model_shader_reference
{
    tag_reference     shader;             /* 0x00 */
    int16_t           permutation_index;  /* 0x10 */
    uint16_t  pad;                /* 0x12 */
    int               unused[3];          /* 0x14 */
} model_shader_reference;                  /* 0x20 (32 bytes) */
