/* get_ui_argb_white @0x83732A08 — fills result with the current UI "white" color: alpha from
 * global_real_argb_white, RGB from the current UI white red/green/blue globals.
 *
 * DEVIATION: disasm shows result->n[1..3] are first written from global_real_argb_white, then
 * immediately overwritten before anything reads them (same dead-store pattern as get_ui_rgb_white).
 * Only the live final values are reproduced; n[0] (alpha) is the one field never overwritten. */

#include "headers/real_argb_color.h"
#include "headers/blam_data_globals.h"


real_argb_color * get_ui_argb_white(real_argb_color *result)
{
    result->n[0] = global_real_argb_white->n[0];
    result->n[1] = global_ui_white_red;
    result->n[2] = global_ui_white_green;
    result->n[3] = global_ui_white_blue;
    return result;
}
