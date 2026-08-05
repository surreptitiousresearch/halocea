#pragma once
/* structure_collision_material — one element of structure_bsp.collision_materials (20 bytes;
 * DB-verified via types_members). Maps a shader tag to a runtime physics material type. */

#include <stdint.h>
#include "tag_reference.h"

typedef struct structure_collision_material
{
    tag_reference    shader;                        /* 0x00 */
    uint16_t pad;                           /* 0x10 */
    int16_t          runtime_physics_material_type; /* 0x12 */
} structure_collision_material;                     /* 20 bytes */
