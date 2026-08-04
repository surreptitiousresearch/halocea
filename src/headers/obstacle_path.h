#pragma once
#include "real_point2d.h"
#include "real_vector2d.h"
#include "step.h"

#include "obstacles.h"
#include "structure_bsp.h" /* full DB layout lives there */

typedef struct obstacle_path
{
    float               radius;                       /* 0x0000 */
    unsigned __int8     ignore_broken_surfaces;       /* 0x0004 */
    unsigned __int8     _pad05[3];
    const obstacles    *obstacles;                    /* 0x0008 */
    const structure_bsp *structure;                   /* 0x000C */
    real_point2d        goal;                         /* 0x0010 (8 bytes) */
    int                 goal_surface_index;            /* 0x0018 */
    __int16             goal_obstacle_index;           /* 0x001C */
    __int16             goal_step_index;               /* 0x001E */
    __int16             best_goal_blocked_step_index;  /* 0x0020 */
    unsigned __int8     _pad22[2];
    float               best_goal_blocked_distance;    /* 0x0024 */
    unsigned __int8     goal_found_exactly;            /* 0x0028 */
    unsigned __int8     finishing;                     /* 0x0029 */
    unsigned __int8     ignore_optional;               /* 0x002A */
    unsigned __int8     _pad2B[1];
    __int16             step_count;                    /* 0x002C */
    unsigned __int8     _pad2E[2];
    step                steps[128];                    /* 0x0030 (5120 bytes) */
    __int16             heap_count;                    /* 0x1430 */
    __int16             heap[128];                     /* 0x1432 (256 bytes) */
    unsigned __int8     _pad1532[2];                   /* trailing struct pad */
} obstacle_path;                                       /* 5428 bytes */
