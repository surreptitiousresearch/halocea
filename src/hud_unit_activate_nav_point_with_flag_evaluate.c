/* hud_unit_activate_nav_point_with_flag_evaluate @0x8372CDE8 — HaloScript builtin wrapper for hud_unit_activate_nav_point_with_flag. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void hud_unit_activate_nav_point_with_flag(int16_t nav_index, int unit_index, int16_t flag_index, float vertical_offset);

void hud_unit_activate_nav_point_with_flag_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        /* HS argument block: 4-byte slots — unit_nav@0, unit@4, flag_index@8, radius@12. */
        hud_unit_activate_nav_point_with_flag(*(__int16 *)arguments, arguments[1], ((__int16 *)arguments)[4], ((float *)arguments)[3]);
        hs_return(thread_index, 0);
    }
}
