#pragma once

/* path_debug_storage — pathfinding debug-visualization sink, passed as an opaque pointer (NULL unless the
 * pathfinding debug overlay is active). Full layout confirmed via types_members (98940 bytes) — embeds a
 * complete snapshot of one actor's last path build: the live path_state, its result, raw/smoothed/avoided
 * step buffers at each stage, and up to 4 obstacle-avoidance passes' obstacles/obstacle_path working sets
 * (path_avoid_obstacles writes into path_obstacles[step]/path_obstacle_paths[step] directly instead of
 * fresh locals when debug_use_stored_obstacles is set, to let the overlay replay each step's obstacle
 * search). */

#include "path_state.h"
#include "path_result.h"
#include "path_step.h"
#include "obstacles.h"
#include "obstacle_path.h"

typedef struct path_debug_storage
{
    int                 actor_index;                  /* 0x00000 */
    int                 path_time;                    /* 0x00004 */
    int                 last_render_id;                /* 0x00008 */
    unsigned __int8     valid;                          /* 0x0000C */
    unsigned __int8     failure;                        /* 0x0000D */
    __int16             structure_bsp_index;            /* 0x0000E */
    __int16             path_traverse_result;           /* 0x00010 */
    __int16             path_build_result;              /* 0x00012 */
    path_state          path_state;                     /* 0x00014 (65676) */
    path_result         result;                          /* 0x100A0 (92) */
    __int16             raw_step_count;                  /* 0x100FC */
    unsigned char _pad0[2]; /* db-verified padding */
    path_step           raw_steps[64];                   /* 0x10100 (1024) */
    __int16             smoothed_step_count;             /* 0x10500 */
    unsigned char _pad1[2]; /* db-verified padding */
    path_step           smoothed_steps[4];                /* 0x10504 (64) */
    __int16             avoided_step_count;               /* 0x10544 */
    unsigned char _pad2[2]; /* db-verified padding */
    path_step           avoided_steps[4];                  /* 0x10548 (64) */
    unsigned __int8     debug_use_stored_obstacles;        /* 0x10588 */
    unsigned char       _pad10589[1];                      /* 0x10589 — db-verified padding */
    __int16             stored_obstacle_step_count;        /* 0x1058A */
    obstacles           path_obstacles[4];                  /* 0x1058C (10272) */
    obstacle_path       path_obstacle_paths[4];               /* 0x12F2C (21712) */
} path_debug_storage; /* 98940 bytes */
