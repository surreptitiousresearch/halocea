#pragma once
#include <stdint.h>
/* converse_state_data — working state for the actor "converse" action (20 bytes). Occupies the front of
 * the actor_datum action_state_data block while the converse action is active. Full DB layout. */

typedef struct converse_state_data
{
    int             conversation_index;   /* 0x00 */
    uint8_t failed;               /* 0x04 */
    uint8_t in_range;             /* 0x05 */
    unsigned char   _pad06[2];            /* 0x06 */
    float           run_to_distance;      /* 0x08 */
    int             run_to_unit_index;    /* 0x0C */
    int             run_to_prop_index;    /* 0x10 */
} converse_state_data;                    /* 20 bytes */
