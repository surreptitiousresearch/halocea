/* video_screen_defaults @0x837852F0 — retail stub: unconditionally returns 1. */

#include <stdint.h>
#include "headers/event_record.h"
#include "headers/widget_instance.h"

uint8_t video_screen_defaults(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    return 1;
}
