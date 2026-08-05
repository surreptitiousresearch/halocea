#pragma once
#include <stdint.h>
/* slayer_variant — slayer game-engine variant settings (3 bytes, DB layout). */

typedef struct slayer_variant
{
    uint8_t no_death_bonus;  /* 0x0 */
    uint8_t no_kill_penalty; /* 0x1 */
    uint8_t kill_in_order;   /* 0x2 */
} slayer_variant;                    /* 3 bytes */
