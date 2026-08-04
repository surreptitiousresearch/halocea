#pragma once
/* dive_animation_possibility — one row of global_dive_animation_table, the const table actor_action_try_to_dive
 * walks to pick an evade/dive animation. 8 bytes; layout from the table data (0x821289D8, each row rendered by
 * IDA as dive_animation_possibility <impulse, direction, weight>). The table is terminated by a row whose
 * animation_impulse == -1. */

typedef struct dive_animation_possibility
{
    __int16 animation_impulse;   /* 0x00 — unit animation-impulse id to request (-1 terminates the table) */
    __int16 animation_direction; /* 0x02 — index (0..3) into the per-direction alignment scores */
    float   weight;              /* 0x04 — additive bias applied to the direction score */
} dive_animation_possibility;    /* 8 bytes */
