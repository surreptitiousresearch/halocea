#pragma once
#include <stdint.h>
/* wait_state_data — per-actor scratch state for the "wait" action, allocated inline in the actor's action
 * data. DB layout, 24 bytes. */

typedef struct wait_state_data
{
    uint8_t wait_done;               /* 0x00 */
    uint8_t waiting_as_coordinator;   /* 0x01 */
    uint8_t was_actively_searching;   /* 0x02 */
    uint8_t desire_move;              /* 0x03 */
    uint8_t move_failed;              /* 0x04 */
    uint8_t _pad05[3];
    int             entry_time;               /* 0x08 */
    int16_t         look_timer;               /* 0x0C */
    int16_t         exit_timer;               /* 0x0E */
    int16_t         query_timer;              /* 0x10 */
    uint8_t _pad12[2];
    int             coordinator_prop_index;   /* 0x14 */
} wait_state_data;                             /* 24 bytes */
