#include "headers/blam_data_globals.h"
/* rasterizer_text_set_shadow_color @0x83768B20 — sets the global drop-shadow color used by
 * rasterizer_draw_character_with_dropshadow (0 = derive shadow from the text color's alpha). */


void rasterizer_text_set_shadow_color(unsigned int color)
{
    global_shadow_color = color;
}
