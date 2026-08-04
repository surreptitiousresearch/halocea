/* hud_unit_deactivate_nav_point_with_object_evaluate @0x8372CF98 — HaloScript builtin wrapper for hud_unit_deactivate_nav_point_with_object. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void hud_unit_deactivate_nav_point_with_object(int unit_index, int object_index);

void hud_unit_deactivate_nav_point_with_object_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        hud_unit_deactivate_nav_point_with_object(*(__int16 *)arguments, arguments[1]);
        hs_return(thread_index, 0);
    }
}
