/* multiplayer_level_list_dispose @0x83692540 — list-widget teardown: detach the item array (owned
 * elsewhere), zero the item count, and free the map preview list. Always reports handled. */
#include <stdint.h>

#include "headers/widget_instance.h"
#include "headers/event_record.h"

extern void preview_list_free(void);

uint8_t multiplayer_level_list_dispose(widget_instance *list_widget, event_record *event,
        unsigned __int8 *widget_deleted)
{
    list_widget->parameters.list_parameters.list_items = nullptr;
    list_widget->parameters.list_parameters.number_of_items = 0;
    preview_list_free();
    return 1;
}
