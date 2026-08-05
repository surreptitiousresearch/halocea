#pragma once
#include <stdint.h>
/* shader_physics_properties — material/physics block of every _shader (DB, 4 bytes). */

typedef struct shader_physics_properties
{
    uint16_t flags;          /* 0x00 */
    int16_t          material_type;  /* 0x02 */
} shader_physics_properties;          /* 0x04 */
