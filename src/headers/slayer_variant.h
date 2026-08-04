#pragma once
/* slayer_variant — slayer game-engine variant settings (3 bytes, DB layout). */

typedef struct slayer_variant
{
    unsigned __int8 no_death_bonus;  /* 0x0 */
    unsigned __int8 no_kill_penalty; /* 0x1 */
    unsigned __int8 kill_in_order;   /* 0x2 */
} slayer_variant;                    /* 3 bytes */
