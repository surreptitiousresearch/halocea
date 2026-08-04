#include "headers/widget_instance.h"

int widget_instance_get_child_index_from_parent(widget_instance *widget)
{
    widget_instance *parent = widget->parent;

    if (!parent)
        return -1;

    widget_instance *child = parent->children;

    if (!child)
        return -1;

    int index = 0;
    while (child != widget)
    {
        child = child->next;
        ++index;
        if (!child)
            return -1;
    }
    return index;
}
