/* ui_widgets_pop_stack @0x83733110 — pops and frees the top saved-state node off a local player's widget
 * stack (local_player_index -1 is treated as player 0). */

#include <stdint.h>
#include "headers/widget_globals.h"
#include "headers/widget_stack_node.h"
#include "headers/stack_memory_pool.h"
#include "headers/blam_data_globals.h"

#include "headers/stack_memory_pool.h"
#include "headers/memory_block.h"
#include "headers/stack_memory_pool.h"
extern void dispose_pointer(stack_memory_pool *pool, void *p);

void ui_widgets_pop_stack(int16_t local_player_index)
{
    int player = local_player_index == -1 ? 0 : local_player_index;
    widget_stack_node *node = widget_globals.widget_stack[player];

    if (node)
    {
        widget_globals.widget_stack[player] = node->next;
        dispose_pointer(widget_memory_pool, node);
    }
}
