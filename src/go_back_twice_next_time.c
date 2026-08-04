/* go_back_twice_next_time @0x83697748 — widget callback: pop the widget stack for this widget's local
 * player so the next "back" navigation collapses two levels. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"

extern void ui_widgets_pop_stack(int16_t local_player_index);

uint8_t go_back_twice_next_time(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    ui_widgets_pop_stack(widget->local_player_index);
    return 1;
}
