/* set_ui_plasma_effect_color @0x837306D0 */
#include "headers/real_argb_color.h"
#include "headers/blam_data_globals.h"


void set_ui_plasma_effect_color(float a, float r, float g, float b)
{
    ui_plasma_effect_color.__s1.alpha = a;   /* alpha+rgb view */
    ui_plasma_effect_color.__s2.red = r;     /* per-channel view */
    ui_plasma_effect_color.__s2.green = g;
    ui_plasma_effect_color.__s2.blue = b;
}
