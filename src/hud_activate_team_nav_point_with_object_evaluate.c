/* hud_activate_team_nav_point_with_object_evaluate @0x8372CEF0 — HaloScript builtin wrapper for hud_activate_team_nav_point_with_object. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void hud_activate_team_nav_point_with_object(int16_t nav_index, int16_t team_index, int object_index, float vertical_offset);

void hud_activate_team_nav_point_with_object_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        /* HS argument block: 4-byte slots — team@0, nav_point@4, object@8, radius@12. */
        hud_activate_team_nav_point_with_object(*(__int16 *)arguments, ((__int16 *)arguments)[2], arguments[2], ((float *)arguments)[3]);
        hs_return(thread_index, 0);
    }
}
