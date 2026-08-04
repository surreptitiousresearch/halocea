/* widget_instance_go_back_to_previous @ 0x83734198 — pop the controller's widget back-stack:
 * delete the current root widget and, if a previous widget was recorded, reload it and restore
 * its focused child. */

#include <stdint.h>
#include "headers/widget_globals.h"
#include "headers/blam_data_globals.h"

#include "headers/stack_memory_pool.h"
#include "headers/memory_block.h"
#include "headers/stack_memory_pool.h"
extern void dispose_pointer(stack_memory_pool *pool, void *p);
extern void ui_widget_delete(widget_instance *widget);
extern widget_instance *ui_widget_load_by_name_or_tag(const char *name, int tag_index, widget_instance *parent, __int16 local_player_index, int invoking_widget_tag, int focused_child_parent_widget_tag, __int16 focused_child_index);
extern void widget_instance_set_focused_child_by_index(widget_instance *widget, int focused_child_parent_widget_tag, int16_t focused_child_index);


void widget_instance_go_back_to_previous(widget_instance *widget)
{
    int slot = (widget->local_player_index == -1) ? 0 : widget->local_player_index;
    widget_stack_node *node = widget_globals.widget_stack[slot];
    int previous_widget_tag = -1;
    int focused_child_parent_widget_tag = 0;
    __int16 restore_controller = -1;
    __int16 focused_child_index = 0;
    widget_instance *root, *p, *reloaded;

    if ( node )
    {
        int packed_focus = node->data.focused_child_index;   /* low=controller, high=child index */
        previous_widget_tag = node->data.previous_widget_tag;
        focused_child_parent_widget_tag = node->data.focused_child_parent_widget_tag;
        widget_globals.widget_stack[slot] = node->next;
        focused_child_index = (__int16)(packed_focus >> 16);
        restore_controller = (__int16)packed_focus;
        dispose_pointer(widget_memory_pool, node);
    }

    /* delete the topmost ancestor (the whole current widget tree) */
    root = widget;
    for ( p = widget->parent; p; p = p->parent )
        root = p;
    ui_widget_delete(root);

    if ( previous_widget_tag != -1 )
    {
        widget_instance_going_back_to_previous = 1;
        reloaded = ui_widget_load_by_name_or_tag(0, previous_widget_tag, 0, restore_controller, -1, -1, -1);
        widget_instance_going_back_to_previous = 0;
        if ( reloaded )
            widget_instance_set_focused_child_by_index(reloaded, focused_child_parent_widget_tag, focused_child_index);
    }
}
