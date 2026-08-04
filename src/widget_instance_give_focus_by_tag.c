/* widget_instance_give_focus_by_tag @ 0x83734140 — find the widget with the given definition
 * tag anywhere in this tree (searching from the root) and give it focus. The third argument is
 * unused here (the decompiler's register was scratch). */

#include <stdint.h>
#include "headers/widget_globals.h"

extern widget_instance *widget_instance_find_by_tag_index_recursive(widget_instance *widget, int tag_index);
extern void widget_instance_give_focus_directly(widget_instance *widget, widget_instance *target);

void widget_instance_give_focus_by_tag(widget_instance *widget, int tag_index, int16_t item_index)
{
    widget_instance *root = widget, *p;
    widget_instance *target;

    (void)item_index;
    for ( p = widget->parent; p; p = p->parent )
        root = p;

    target = widget_instance_find_by_tag_index_recursive(root, tag_index);
    if ( target )
        widget_instance_give_focus_directly(root, target);
}
