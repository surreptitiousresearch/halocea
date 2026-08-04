/* disable_widget_if_no_xdemos @0x836972E8 — widget callback: if no Xbox demos are available, make the
 * widget non-interactive and invisible. Always returns 1. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"

extern uint8_t xbox_demos_available(void);

uint8_t disable_widget_if_no_xdemos(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    if ( !xbox_demos_available() )
    {
        widget->never_receive_events = 1;
        widget->visible = 0;
    }
    return 1;
}
