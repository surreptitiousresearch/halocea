#pragma once
/* dialogue_event_status — per (dialogue-table-row, communication-team) runtime throttle record in
 * global_dialogue_events. Tracks when a line was last spoken and until when it is disabled. 8 bytes. */

typedef struct dialogue_event_status
{
    int last_time_spoken;    /* 0x00 — game time, or -1 if never */
    int disable_until_time;  /* 0x04 — game time the line is suppressed until, or -1 */
} dialogue_event_status;     /* 8 bytes */
