/* event_manager_queue_custom_activation_event @0x83726870 — DEVIATION: the decompiler builds the
 * event_record via one raw 64-bit constant (0x0005000500000000) written to a stack temp and reinterpreted
 * as the struct; decomposed per event_record's known field layout (big-endian) this is
 * { type = 5, controller_index = 5, value = 0 }. */

#include <stdint.h>
#include "headers/event_record.h"
#include "headers/event_type.h"

extern void queue_event(event_record *event, int16_t local_player_index);

void event_manager_queue_custom_activation_event(int16_t player_number)
{
    event_record event;
    event.type = event_type_custom_activation;
    event.controller_index = 5;
    event.value.value = 0;
    queue_event(&event, player_number);
}
