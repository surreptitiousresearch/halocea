#pragma once
/* avoid_state_data — per-actor "avoid" action scratch state (4 bytes). DB layout: a single reserved/unused
 * field; action_avoid_setup only ever zeroes it. */

typedef struct avoid_state_data
{
    int pad; /* 0x00 */
} avoid_state_data; /* 4 bytes */
