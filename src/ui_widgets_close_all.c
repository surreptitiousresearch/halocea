/* ui_widgets_close_all @0x83732FB8 — tear down every widget on the widget stack: delete each live widget
 * instance and dispose each occupied stack slot, then clear the pause-time counter and cancel config mode.
 *
 * The stack is walked as an array of nodes up to the address of current_system_milliseconds (the field that
 * immediately follows the stack array in widget_globals); the instance pointer sits 2 slots before each node. */

#include "headers/widget_globals.h"
/* DEVIATION: the decompiler modeled widget_globals with widget_stack@0; the DB-verified layout has
 * active_widgets[2]@0 then widget_stack[2]@8 — so the `*(widget_stack - 2)` instance pointer correctly
 * reads active_widgets, and the walk runs to &current_system_milliseconds@0x10. Reusing the canonical
 * header (widget_globals.h) corrects the offsets. */

extern void ui_widget_delete(widget_instance *widget);
extern void dispose_widget_stack(widget_stack_node **top);
extern void controls_screen_cancel_config_mode(void);

void ui_widgets_close_all(void) /* attested void: 0/11 callers consume r3 */
{
    widget_stack_node **widget_stack = widget_globals.widget_stack;

    do
    {
        widget_instance *instance = (widget_instance *)*(widget_stack - 2);
        if ( instance )
            ui_widget_delete(instance);
        if ( *widget_stack )
            dispose_widget_stack(widget_stack);
        ++widget_stack;
    }
    while ( (int)widget_stack < (int)&widget_globals.current_system_milliseconds );

    widget_globals.pause_game_time_count = 0;
    controls_screen_cancel_config_mode(); /* attested void: tail-call, 0/11 callers consume r3 */
}
