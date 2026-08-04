#include <stdint.h>
#include "headers/widget_instance.h"

void widget_instance_set_visibility_recursive(widget_instance *widget, uint8_t visible)
{
    widget->visible = visible;
    for (widget_instance *child = widget->children; child; child = child->next)
        widget_instance_set_visibility_recursive(child, visible);
}
