#include <stdint.h>
#include "../headers/ui_widget_replace_function.h" /* typedef moved to header so the canonical globals header can name the type */

/* .data init @0x84185408:
 * { &widget_replace_function_null, &widget_replace_function_build,
 *   &widget_replace_function_pid, &widget_controller } */
#include "headers/widget_instance.h"
extern uint16_t * widget_replace_function_null(widget_instance *widget);
extern uint16_t * widget_replace_function_build(widget_instance *widget);
extern uint16_t * widget_replace_function_pid(widget_instance *widget);
extern uint16_t * widget_controller(widget_instance *widget);

ui_widget_replace_function replace_function_list[4] =
{
    widget_replace_function_null,
    widget_replace_function_build,
    widget_replace_function_pid,
    widget_controller,
};
