/* video_test_dialog_dispose @0x837854E8 — retail stub: unconditionally returns 1. */

#include <stdint.h>
#include "headers/event_record.h"
#include "headers/widget_instance.h"

uint8_t video_test_dialog_dispose(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    return 1;
}
