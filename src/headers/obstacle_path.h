#pragma once
#include <stdint.h>
#include "real_point2d.h"
#include "real_vector2d.h"
#include "step.h"

#include "obstacles.h"
#include "structure_bsp.h" /* full DB layout lives there */

typedef struct obstacle_path
{
    float               radius;                       /* 0x0000 */
    uint8_t     ignore_broken_surfaces;       /* 0x0004 */
    uint8_t     _pad05[3];
    const obstacles    *obstacles;                    /* 0x0008 */
    const structure_bsp *structure;                   /* 0x000C */
    real_point2d        goal;                         /* 0x0010 (8 bytes) */
    int                 goal_surface_index;            /* 0x0018 */
    int16_t             goal_obstacle_index;           /* 0x001C */
    int16_t             goal_step_index;               /* 0x001E */
    int16_t             best_goal_blocked_step_index;  /* 0x0020 */
    uint8_t     _pad22[2];
    float               best_goal_blocked_distance;    /* 0x0024 */
    uint8_t     goal_found_exactly;            /* 0x0028 */
    uint8_t     finishing;                     /* 0x0029 */
    uint8_t     ignore_optional;               /* 0x002A */
    uint8_t     _pad2B[1];
    int16_t             step_count;                    /* 0x002C */
    uint8_t     _pad2E[2];
    step                steps[128];                    /* 0x0030 (5120 bytes) */
    int16_t             heap_count;                    /* 0x1430 */
    int16_t             heap[128];                     /* 0x1432 (256 bytes) */
    uint8_t     _pad1532[2];                   /* trailing struct pad */
} obstacle_path;                                       /* 5428 bytes */
