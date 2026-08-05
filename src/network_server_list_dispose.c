#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"

uint8_t network_server_list_dispose(widget_instance *widget,
                                 event_record *event,
                                 uint8_t *widget_deleted)
{
    widget->parameters.list_parameters.list_items    = 0;
    widget->parameters.list_parameters.number_of_items = 0;
    return 1;
}
