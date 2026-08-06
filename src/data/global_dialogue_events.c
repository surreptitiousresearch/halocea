/* global_dialogue_events @ 0x844A6198 (.data, 4 bytes)
 * DB applied_types: dialogue_event_status *global_dialogue_events;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x00000000
 * global_dialogue_events — global dialogue events pointer (data-bss, 4)
 */
#include "../headers/dialogue_event_status.h"

dialogue_event_status *global_dialogue_events;
