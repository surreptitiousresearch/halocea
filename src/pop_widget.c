#include "headers/widget_stack_node.h"
#include "headers/blam_data_globals.h"

#include "headers/stack_memory_pool.h"
#include "headers/memory_block.h"
#include "headers/stack_memory_pool.h"
extern void dispose_pointer(stack_memory_pool *pool, void *p);

void pop_widget(widget_stack_node **top, _widget_stack_node_data *data)
{
    widget_stack_node *node = *top;
    *data = node->data;
    *top = node->next;
    dispose_pointer(widget_memory_pool, node);
}
