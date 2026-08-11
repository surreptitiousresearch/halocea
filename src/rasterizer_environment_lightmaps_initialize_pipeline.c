/* rasterizer_environment_lightmaps_initialize_pipeline @ 0x8378F5F0 */

#include "headers/rasterizer_draw_pipeline.h"

void rasterizer_environment_lightmaps_initialize_pipeline(void)
{
    rasterizer_environment_lightmaps_draw_function = _rasterizer_environment_lightmap_draw_pp;
}
