/* event_manager_queue_up_event @0x837265F8 — queue a "up" UI input event (button code 8) for a
 * local player, when the event manager is initialized. */

#include <stdint.h>
#include "headers/event_record.h"
#include "headers/event_manager_globals.h"
#include "headers/event_type.h"
#include "headers/gamepad_button.h"

extern void queue_event(event_record *event, int16_t local_player_index);

void event_manager_queue_up_event(uint8_t ticks, int16_t player_number)
{
    if (event_manager_globals.initialized)
    {
        event_record event;
        event.type = event_type_button;
        event.value.button_value.button = _gamepad_binary_button_dpad_up;
        event.value.button_value.value = ticks;
        queue_event(&event, player_number);
    }
}
