/* replace_function_list @ 0x84185408 (.data, 16 bytes)
 * DB applied_types: unsigned __int16 *(__fastcall *replace_function_list[4])(widget_instance *);
 * UI text-replacement dispatch, indexed by a widget definition's replace-function selector.
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x83803FA8 -> widget_replace_function_null
 *   +0x0004 = 0x83804078 -> widget_controller
 *   +0x0008 = 0x83803FB8 -> widget_replace_function_build
 *   +0x000C = 0x83804020 -> widget_replace_function_pid
 * DEVIATION: the previous reconstruction listed { null, build, pid, controller } — the last three
 * rotated by one against the image. Because the table is indexed by the widget's stored selector
 * rather than walked, every widget with selector 1..3 resolved to the wrong replacement function.
 * Caught 2026-08-06 by data_provenance.py --verify against the .data record. */
#include <stdint.h>
#include "../headers/ui_widget_replace_function.h" /* typedef moved to header so the canonical globals header can name the type */

#include "headers/widget_instance.h"
extern uint16_t * widget_replace_function_null(widget_instance *widget);
extern uint16_t * widget_replace_function_build(widget_instance *widget);
extern uint16_t * widget_replace_function_pid(widget_instance *widget);
extern uint16_t * widget_controller(widget_instance *widget);

ui_widget_replace_function replace_function_list[4] =
{
    widget_replace_function_null,
    widget_controller,
    widget_replace_function_build,
    widget_replace_function_pid,
};
