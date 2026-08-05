#pragma once
/* render_skinning — the skinned node-matrix palette handed to the rasterizer when drawing a model's parts.
 * Layout from the database (8 bytes incl. padding). */

#include <stdint.h>
#include "real_matrix4x3.h"

typedef struct render_skinning
{
    real_matrix4x3 *node_matrices;     /* 0x00 */
    int16_t         node_matrix_count; /* 0x04 */
} render_skinning;
