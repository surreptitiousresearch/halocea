/* push_widget @0x83731230 — push a new saved-state node onto a widget stack. */

#include "headers/widget_stack_node.h"
#include "headers/widget_globals.h"

extern void *pool_new_pointer(stack_memory_pool *pool, unsigned int size);

void push_widget(widget_stack_node **top, _widget_stack_node_data *data)
{
    widget_stack_node *node = pool_new_pointer(widget_memory_pool, 0x10u);

    if ( node )
    {
        node->data = *data;
        node->next = *top;
        *top = node;
    }
}
