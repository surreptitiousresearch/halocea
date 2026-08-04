/* event_manager_queue_create_event @0x837267D8 — queue a "create" UI input event (button code 3) for a
 * local player, when the event manager is initialized. */

#include <stdint.h>
#include "headers/event_record.h"
#include "headers/event_manager_globals.h"
#include "headers/event_type.h"
#include "headers/gamepad_button.h"

extern void queue_event(event_record *event, int16_t local_player_index);

void event_manager_queue_create_event(uint8_t ticks, int16_t player_number)
{
    if (event_manager_globals.initialized)
    {
        event_record event;
        event.type = event_type_button;
        event.value.button_value.button = _gamepad_analog_button_y;
        event.value.button_value.value = ticks;
        queue_event(&event, player_number);
    }
}
