/* ui_widget_debug_show_path_evaluate @0x8372DEA8 — HaloScript builtin wrapper for ui_widget_debug_show_path; (bool). */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void ui_widget_debug_show_path(uint8_t show);

void ui_widget_debug_show_path_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    unsigned char *arguments = (unsigned char *)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        ui_widget_debug_show_path(*arguments);
        hs_return(thread_index, 0);
    }
}
