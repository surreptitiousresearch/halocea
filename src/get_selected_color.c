/* get_selected_color @0x837465F8 — fixed near-white "selected item" highlight color used by UI widgets. */

#include "headers/real_argb_color.h"

void get_selected_color(real_argb_color *selected_color)
{
    selected_color->alpha = 1.0f;
    selected_color->rgb.red = 0.98000002f;
    selected_color->rgb.green = 0.95999998f;
    selected_color->rgb.blue = 0.95999998f;
}
