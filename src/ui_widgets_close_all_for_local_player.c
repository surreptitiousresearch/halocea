/* ui_widgets_close_all_for_local_player @0x83733030 */
#include <stdint.h>
#include "headers/widget_globals.h"

extern void ui_widget_delete(widget_instance *widget);
extern void dispose_widget_stack(widget_stack_node **top);

void ui_widgets_close_all_for_local_player(int16_t local_player_index)
{
    int slot_index;

    /* recovered: cursor pointer-walk -> active_widgets[2]/widget_stack[2] index loop */
    for ( slot_index = 0; slot_index < 2; ++slot_index )
    {
        widget_instance *widget = widget_globals.active_widgets[slot_index];

        if ( widget && widget->local_player_index == local_player_index )
        {
            ui_widget_delete(widget);
            if ( widget_globals.widget_stack[slot_index] )
                dispose_widget_stack(&widget_globals.widget_stack[slot_index]);
        }
    }
}
