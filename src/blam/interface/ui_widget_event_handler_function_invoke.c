/* ui_widget_event_handler_function_invoke @ 0x836918E8 — dispatch to a widget event-handler
 * function by index through the function table. The 0x8000 bit (and indices >= 0x6B) mark
 * "no function"; those return 0. */

#include <stdint.h>
#include "headers/widget_globals.h"
#include "headers/event_record.h"

extern uint8_t (*event_handler_function_list[107])(widget_instance *widget, event_record *event, uint8_t *widget_deleted);

/* Return is 8-bit: both consuming callers normalize r3 with `clrlwi r11,r3,24` (byte-norm rule). */
uint8_t ui_widget_event_handler_function_invoke(widget_instance *widget, event_record *event,
                                                uint16_t function, unsigned char *widget_deleted)
{
    if ( (int16_t)function < 0 || function >= 0x6Bu )
        return 0;
    return event_handler_function_list[(int16_t)function](widget, event, widget_deleted);
}
