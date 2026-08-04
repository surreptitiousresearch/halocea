#pragma once
/* actor_idle_looking — idle look/aim/facing timing bounds from the actor variant definition.
 * One of three such substructs is selected per actor by its looking mode (def+220/+244/+268).
 * 24 bytes, full DB layout. */

typedef struct actor_idle_looking
{
    float idle_facing_time_lower_bound;   /* 0x00 */
    float idle_facing_time_upper_bound;   /* 0x04 */
    float idle_aim_time_lower_bound;      /* 0x08 */
    float idle_aim_time_upper_bound;      /* 0x0C */
    float idle_look_time_lower_bound;     /* 0x10 */
    float idle_look_time_upper_bound;     /* 0x14 */
} actor_idle_looking;
