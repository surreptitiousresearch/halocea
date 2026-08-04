#pragma once
/* ui_widget_replace_function — one entry of replace_function_list[4] (ui .data table @0x84185408):
 * a text-replacement callback returning the replacement wide string for a widget. */

#include "widget_instance.h"

typedef unsigned __int16 *(*ui_widget_replace_function)(widget_instance *widget);

extern ui_widget_replace_function replace_function_list[4];
