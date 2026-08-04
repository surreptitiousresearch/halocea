#pragma once
/* physics_variable — a definition block for a seek-toward-target scalar that also tracks its own
 * speed (24 bytes). This is a real PDB type (types_members-confirmed). The trailing 16 bytes
 * (maximum_speed/minimum_speed/acceleration/deceleration) are reinterpreted as a physics_variable_speed
 * by physics_variable_update, matching the disasm-confirmed `&definition->maximum_speed` cast. */

typedef struct physics_variable
{
    float maximum_position;  /* 0x0 */
    float minimum_position;  /* 0x4 */
    float maximum_speed;     /* 0x8 */
    float minimum_speed;     /* 0xC */
    float acceleration;      /* 0x10 */
    float deceleration;      /* 0x14 */
} physics_variable;           /* 24 bytes */
