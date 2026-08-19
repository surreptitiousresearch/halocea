/* dispose_widget_stack @0x837312D8 — free every node in a widget stack list back to the widget memory pool. */

#include "headers/widget_stack_node.h"
#include "headers/blam_data_globals.h"

#include "headers/stack_memory_pool.h"
#include "headers/memory_block.h"
#include "headers/stack_memory_pool.h"
extern void dispose_pointer(stack_memory_pool *pool, void *p);

void dispose_widget_stack(widget_stack_node **top)
{
    while ( *top )
    {
        widget_stack_node *node = *top;
        *top = (*top)->next;
        dispose_pointer(widget_memory_pool, node);
    }
}
