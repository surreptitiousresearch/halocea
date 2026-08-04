/* ui_widget_search_and_replace_invoke @0x83803F68 — dispatches a text-box "$-function" placeholder
 * (index 0-3) to its handler, each of which returns a replacement string for the widget. Any out-of-range
 * function index (including the high-bit "invalid" sentinel range) yields the literal "<invalid>". */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/blam_data_globals.h"

typedef uint16_t *(*ui_widget_replace_function)(widget_instance *widget);

uint16_t *ui_widget_search_and_replace_invoke(widget_instance *widget, int16_t function)
{
    if (function < 0 || (uint16_t)function >= 4)
        return L"<invalid>";
    return replace_function_list[(uint16_t)function](widget);
}
