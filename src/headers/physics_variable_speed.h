#pragma once
/* physics_variable_speed — a definition block describing a seek-toward-target scalar with
 * acceleration/deceleration limits (16 bytes). This is a real PDB type (types_members-confirmed).
 * FIX: acceleration/deceleration were previously swapped (0x8/0xC reversed) from a mis-derivation;
 * corrected to match the ground-truth PDB member offsets. physics_variable_speed_update.c reads
 * these fields by name, so no other file needed to change. */

typedef struct physics_variable_speed
{
    float maximum_forward_speed;  /* 0x0 */
    float maximum_reverse_speed;  /* 0x4 */
    float acceleration;           /* 0x8 */
    float deceleration;           /* 0xC */
} physics_variable_speed;         /* 16 bytes */
