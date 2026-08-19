/* multiplayer_profiles_list_dispose @0x83692810 — list-widget teardown: free the widget-owned item
 * array, clear the item count, and free the preview list. Always reports handled. */
#include <stdint.h>

#include "headers/widget_instance.h"
#include "headers/event_record.h"

extern void widget_free(void *ptr);
extern void preview_list_free(void);

uint8_t multiplayer_profiles_list_dispose(widget_instance *list_widget, event_record *event,
        uint8_t *widget_deleted)
{
    void *list_items = list_widget->parameters.list_parameters.list_items;
    if ( list_items )
    {
        widget_free(list_items);
        list_widget->parameters.list_parameters.list_items = nullptr;
    }
    list_widget->parameters.list_parameters.number_of_items = 0;
    preview_list_free();
    return 1;
}
