#pragma once
/* director_variable_instance — a single smoothed debug-camera variable (value + its velocity and
 * per-frame delta). The director keeps an array of these for debug camera tuning. */

typedef struct director_variable_instance
{
    float value;                            /* 0x00 */
    float velocity;                         /* 0x04 */
    float delta;                            /* 0x08 */
} director_variable_instance;               /* 12 bytes */
