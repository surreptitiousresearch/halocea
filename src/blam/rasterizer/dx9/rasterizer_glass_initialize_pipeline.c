/* rasterizer_glass_initialize_pipeline @ 0x8378E770 */

#include "headers/rasterizer_draw_pipeline.h"

void rasterizer_glass_initialize_pipeline(void)
{
    rasterizer_glass_draw_tint = rasterizer_glass_draw_tint_pp;
    rasterizer_glass_draw_reflection = rasterizer_glass_draw_reflection_pp;
    rasterizer_glass_draw_diffuse = rasterizer_glass_draw_diffuse_pp;
}
