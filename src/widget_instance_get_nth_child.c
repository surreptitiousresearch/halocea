/* widget_instance_get_nth_child @0x83730810 — the n-th widget in widget's children list (0 = first), or
 * NULL if the list is shorter than n. */

#include "headers/widget_instance.h"

widget_instance *widget_instance_get_nth_child(widget_instance *widget, int n)
{
    widget_instance *result = widget->children;
    for (int i = 0; i < n && result; ++i)
        result = result->next;
    return result;
}
