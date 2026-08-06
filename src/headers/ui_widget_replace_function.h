#pragma once
/* ui_widget_replace_function — one entry of replace_function_list[4] (ui .data table @0x84185408):
 * a text-replacement callback returning the replacement wide string for a widget. */

#include <stdint.h>
#include "widget_instance.h"

typedef uint16_t *(*ui_widget_replace_function)(widget_instance *widget);

#ifdef __cplusplus
extern "C" {
#endif

extern ui_widget_replace_function replace_function_list[4];

#ifdef __cplusplus
}
#endif
