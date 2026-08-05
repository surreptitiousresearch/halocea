/* hud_activate_team_nav_point_with_flag_evaluate @0x8372CE98 — HaloScript builtin wrapper for hud_activate_team_nav_point_with_flag. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void hud_activate_team_nav_point_with_flag(int16_t nav_index, int16_t team_index, int16_t flag_index, float vertical_offset);

void hud_activate_team_nav_point_with_flag_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        /* HS argument block: 4-byte slots — team@0, nav_point@4, flag_index@8, radius@12. */
        hud_activate_team_nav_point_with_flag(*(int16_t *)arguments, ((int16_t *)arguments)[2], ((int16_t *)arguments)[4], ((float *)arguments)[3]);
        hs_return(thread_index, 0);
    }
}
