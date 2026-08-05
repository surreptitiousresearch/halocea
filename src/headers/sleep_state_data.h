#pragma once
#include <stdint.h>
/* sleep_state_data — actor "sleep" action_data payload (2 bytes, types_members-confirmed layout). The sleep
 * action carries no working state beyond alignment padding. */

typedef struct sleep_state_data
{
    uint16_t pad;   /* 0x00 */
} sleep_state_data;         /* 2 bytes */
