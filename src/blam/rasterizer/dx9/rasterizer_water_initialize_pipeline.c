/* rasterizer_water_initialize_pipeline @ 0x837889B8
   Selects the per-pixel water draw implementation. */

#include "headers/rasterizer_draw_pipeline.h"

void rasterizer_water_initialize_pipeline(void)
{
    rasterizer_water_draw = rasterizer_water_draw_pp;
}
