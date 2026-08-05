#pragma once
#include <stdint.h>
#include "real_point2d.h"
#include "real_vector2d.h"

typedef struct step
{
    real_point2d    point;                          /* 0x00 (8 bytes) */
    int             surface_index;                  /* 0x08 */
    real_vector2d   direction;                      /* 0x0C (8 bytes) */
    float           distance;                       /* 0x14 */
    int16_t         obstacle_index;                 /* 0x18 */
    uint8_t obstacle_direction_index;       /* 0x1A */
    uint8_t _pad1B[1];
    int16_t         obstructed_goal_step_indices[2];/* 0x1C */
    float           total_distance;                 /* 0x20 */
    int16_t         previous_step_index;            /* 0x24 */
    uint8_t _pad26[2];                      /* trailing pad to 40 bytes */
} step;                                             /* 40 bytes */
