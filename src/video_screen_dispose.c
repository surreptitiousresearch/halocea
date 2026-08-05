/* video_screen_dispose @0x837852B8 — clears the list_items pointer on two specific descendants of the video
 * screen widget's tree (their owning list widgets are about to be torn down); `event`/`widget_deleted` are
 * unused. Always reports success. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"

uint8_t video_screen_dispose(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    widget_instance *children = widget->children;
    widget_instance *next = children->next->children->next;

    children->children->next->parameters.list_parameters.list_items = 0;
    next->parameters.list_parameters.list_items = 0;

    return 1;
}
