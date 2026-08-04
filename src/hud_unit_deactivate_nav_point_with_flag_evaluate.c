/* hud_unit_deactivate_nav_point_with_flag_evaluate @0x8372CF48 — HaloScript builtin wrapper for hud_unit_deactivate_nav_point_with_flag. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void hud_unit_deactivate_nav_point_with_flag(int unit_index, int16_t flag_index);

void hud_unit_deactivate_nav_point_with_flag_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        /* HS argument block: 4-byte slots — unit_nav@0, flag_index@4. */
        hud_unit_deactivate_nav_point_with_flag(*(__int16 *)arguments, ((__int16 *)arguments)[2]);
        hs_return(thread_index, 0);
    }
}
