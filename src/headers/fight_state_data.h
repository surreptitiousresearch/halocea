#pragma once
#include <stdint.h>
/* fight_state_data — per-actor "fight" action scratch state (4 bytes). DB layout. */

typedef struct fight_state_data
{
    int16_t          firing_position_timer; /* 0x00 */
    uint16_t pad;                   /* 0x02 */
} fight_state_data; /* 4 bytes */
