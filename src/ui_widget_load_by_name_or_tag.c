/* ui_widget_load_by_name_or_tag @ 0x83736538 — instantiate a UI widget by tag (or resolve
 * a name to a 'DeLa' tag first). When loading a top-level widget (no parent) it replaces the
 * controller's current root widget, optionally pushing the invoking widget onto the back
 * stack so it can be restored. Picks a controller index from the widget's player count when
 * the caller passed -1. Returns the new widget instance, or null on failure. */

#include <stdint.h>
#include "headers/widget_globals.h"
#include "headers/global_tag_instances.h"
#include "headers/ui_widget_definition.h"
#include "headers/ui_widget_definition_flags.h"
#include "headers/blam_data_globals.h"

extern int tag_loaded(uint32_t group_tag, const char *name);
extern void *pool_new_pointer(stack_memory_pool *pool, unsigned int size);
extern void ui_widget_delete(widget_instance *widget);
/* extern corrected to match def (widget_instance_initialize.c): 5th arg is ui_widget_definition*. */
extern void widget_instance_initialize(widget_instance *widget, widget_instance *parent, int ui_widget_definition_index, int16_t local_player_index, ui_widget_definition *definition, int back_inhibited);


widget_instance *ui_widget_load_by_name_or_tag(const char *name, int tag_index, widget_instance *parent,
                                               int16_t local_player_index, int invoking_widget_tag,
                                               int focused_child_parent_widget_tag,
                                               int16_t focused_child_index)
{
    int16_t controller_index = local_player_index;
    int16_t stack_controller;
    ui_widget_definition *definition_data;   /* retyped from int: the tag slot stores a pointer (disasm reads a word ptr) */
    widget_instance *widget;
    int back_inhibited = 0;   /* holds flags & _widget_dont_push_history_data_bit (a boolean); passed to initialize which ignores it */

    should_force_hot_update = 1;
    stack_controller = (local_player_index == -1) ? 0 : local_player_index;

    if ( tag_index == -1 )
    {
        tag_index = tag_loaded(0x44654C61u /* 'DeLa' */, name);
        if ( tag_index == -1 )
            return 0;
    }

    definition_data = TAG_GET(ui_widget_definition, tag_index);
    widget = pool_new_pointer(widget_memory_pool, 0x60u);
    if ( !widget )
        return widget;

    if ( !parent )
    {
        widget_instance *current = widget_globals.active_widgets[stack_controller];
        int16_t previous_controller;

        if ( current )
        {
            previous_controller = current->local_player_index;
            ui_widget_delete(current);
        }
        else
        {
            previous_controller = -1;
        }
        widget_globals.active_widgets[stack_controller] = widget;

        if ( invoking_widget_tag != -1 )
        {
            /* don't push a back-stack entry if the invoking widget requests it (flags bit 14) */
            /* DEVIATION FIX: restored missing deref — DB reads (*(_DWORD **)slot)[11] = definition
             * flags dword (0x2C), not the table slot's address */
            back_inhibited =
                TAG_GET(ui_widget_definition, invoking_widget_tag)->flags
                 & (1u << _widget_dont_push_history_data_bit);
            if ( !back_inhibited )
            {
                widget_stack_node *node;

                node = pool_new_pointer(widget_memory_pool, 0x10u);
                if ( node )
                {
                    node->data.previous_widget_tag = invoking_widget_tag;
                    node->data.focused_child_parent_widget_tag = focused_child_parent_widget_tag;
                    /* DEVIATION: decompiler packed both int16s into one dword store; split into
                     * the two members (focused_child_index @0x8 = BE high half, local_player_index
                     * @0xA = low half — same bytes stored) */
                    node->data.focused_child_index = focused_child_index;
                    node->data.local_player_index = previous_controller;
                    node->next = widget_globals.widget_stack[stack_controller];
                    widget_globals.widget_stack[stack_controller] = node;
                }
            }
        }
    }

    /* resolve a concrete controller from the widget's player count when caller passed -1 */
    if ( local_player_index == -1 )
    {
        unsigned int player_count = (uint16_t)definition_data->controller_index;
        if ( player_count <= 4 )
        {
            if ( player_count == 1 )
                controller_index = 1;
            else if ( player_count == 2 )
                controller_index = 2;
            else if ( player_count == 3 && definition_data->controller_index )
                controller_index = 3;
            else if ( definition_data->controller_index )
                controller_index = -1;
            else
                controller_index = 0;
        }
    }

    widget_instance_initialize(widget, parent, tag_index, controller_index,
                               definition_data, back_inhibited);
    return widget;
}
