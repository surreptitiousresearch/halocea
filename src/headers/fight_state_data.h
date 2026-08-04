#pragma once
/* fight_state_data — per-actor "fight" action scratch state (4 bytes). DB layout. */

typedef struct fight_state_data
{
    __int16          firing_position_timer; /* 0x00 */
    unsigned __int16 pad;                   /* 0x02 */
} fight_state_data; /* 4 bytes */
