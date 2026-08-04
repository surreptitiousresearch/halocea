#include "headers/widget_globals.h"

#include "headers/stack_memory_pool.h"
#include "headers/memory_block.h"
#include "headers/stack_memory_pool.h"
extern void dispose_pointer(stack_memory_pool *pool, void *p);

void ui_widgets_delete_history(void)
{
    int stack_index;

    /* recovered: stack pointer-walk bounded by &current_system_milliseconds -> widget_stack[2] index loop */
    for ( stack_index = 0; stack_index < 2; ++stack_index )
    {
        widget_stack_node **stack = &widget_globals.widget_stack[stack_index];

        while ( *stack )
        {
            widget_stack_node *node = *stack;

            *stack = node->next;
            dispose_pointer(widget_memory_pool, node);
        }
    }
}
