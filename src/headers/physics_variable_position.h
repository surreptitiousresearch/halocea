#pragma once
/* physics_variable_position — a definition block giving the inclusive range a seek-toward-target
 * variable position is allowed to occupy (8 bytes, DB layout). When seeking is cyclical the range
 * wraps around between minimum and maximum. */

typedef struct physics_variable_position
{
    float maximum_position;   /* 0x0 */
    float minimum_position;   /* 0x4 */
} physics_variable_position;  /* 8 bytes */
