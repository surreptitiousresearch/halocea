/* get_ui_rgb_white @0x837329C0 — fills result with the current UI "white" color (red/green/blue globals).
 *
 * DEVIATION: disasm shows result is first written from global_real_rgb_white's private_real_argb_colors
 * fields, then immediately overwritten in place by the float stores below before anything reads it —
 * genuinely dead stores in the original binary. Omitted here; only the final values are reproduced. */

#include "headers/real_rgb_color.h"
#include "headers/blam_data_globals.h"


real_rgb_color *get_ui_rgb_white(real_rgb_color *result)
{
    result->n[0] = global_ui_white_red;
    result->n[1] = global_ui_white_green;
    result->n[2] = global_ui_white_blue;
    return result;
}
