#pragma once
/* wait_state_data — per-actor scratch state for the "wait" action, allocated inline in the actor's action
 * data. DB layout, 24 bytes. */

typedef struct wait_state_data
{
    unsigned __int8 wait_done;               /* 0x00 */
    unsigned __int8 waiting_as_coordinator;   /* 0x01 */
    unsigned __int8 was_actively_searching;   /* 0x02 */
    unsigned __int8 desire_move;              /* 0x03 */
    unsigned __int8 move_failed;              /* 0x04 */
    unsigned __int8 _pad05[3];
    int             entry_time;               /* 0x08 */
    __int16         look_timer;               /* 0x0C */
    __int16         exit_timer;               /* 0x0E */
    __int16         query_timer;              /* 0x10 */
    unsigned __int8 _pad12[2];
    int             coordinator_prop_index;   /* 0x14 */
} wait_state_data;                             /* 24 bytes */
