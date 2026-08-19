/* hs_print @0x837F7788 — print a string to the console in green (HaloScript print builtin). */

#include "headers/real_argb_color.h"
#include "headers/blam_data_globals.h"

extern void terminal_printf(const real_argb_color *color, const char *format, ...);

void hs_print(const char *string)
{
    terminal_printf(global_real_argb_green, string);
}
