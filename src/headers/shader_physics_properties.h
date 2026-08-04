#pragma once
/* shader_physics_properties — material/physics block of every _shader (DB, 4 bytes). */

typedef struct shader_physics_properties
{
    unsigned __int16 flags;          /* 0x00 */
    __int16          material_type;  /* 0x02 */
} shader_physics_properties;          /* 0x04 */
