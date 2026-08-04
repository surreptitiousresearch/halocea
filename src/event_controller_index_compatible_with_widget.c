/* event_controller_index_compatible_with_widget @0x837306A0 — an event is deliverable to a widget when
 * the widget isn't bound to a specific local player (-1), or when the event's controller matches the
 * widget's local player index. */

#include <stdint.h>
#include "headers/event_record.h"
#include "headers/widget_instance.h"

uint8_t event_controller_index_compatible_with_widget(event_record *event, widget_instance *widget)
{
    if ( widget->local_player_index == -1 )
        return 1;
    return widget->local_player_index == event->controller_index;
}
