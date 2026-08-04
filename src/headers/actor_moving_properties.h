#pragma once
/* actor_moving_properties — actor_definition.moving (36 bytes, DB types_members-confirmed layout). */

typedef struct actor_moving_properties
{
    float cover_dive_chance;                    /* 0x00 */
    float cover_emerge_chance;                  /* 0x04 */
    float grenade_dive_chance;                  /* 0x08 */
    float pathfinding_radius;                   /* 0x0C */
    float glass_ignorance_chance;                /* 0x10 */
    float stationary_moving_distance;            /* 0x14 */
    float free_flying_sidestep_distance;         /* 0x18 */
    float begin_movement_angle;                  /* 0x1C */
    float runtime_begin_movement_angle_cosine;   /* 0x20 */
} actor_moving_properties; /* 36 bytes */
