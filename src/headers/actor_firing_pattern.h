#pragma once

/* actor_firing_pattern — per-target firing pattern modifiers (24 bytes, DB-verified layout). */
typedef struct actor_firing_pattern
{
    float burst_duration_modifier;      /* 0x00 */
    float burst_separation_modifier;    /* 0x04 */
    float rate_of_fire_modifier;        /* 0x08 */
    float error_angle_modifier;         /* 0x0C */
    unsigned int unused[2];             /* 0x10 */
} actor_firing_pattern;                 /* 24 bytes */
