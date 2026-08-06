/* global_reply_events @ 0x844A61A0 (.data, 4 bytes)
 * DB applied_types: dialogue_event_status *global_reply_events;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x00000000
 * global_reply_events — global reply events pointer (data-bss, 4)
 */
#include "../headers/dialogue_event_status.h"

dialogue_event_status *global_reply_events;
