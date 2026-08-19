/* rasterizer_environment_diffuse_lights_initialize_pipeline @ 0x837897F0 */

#include "headers/rasterizer_draw_pipeline.h"

void rasterizer_environment_diffuse_lights_initialize_pipeline(void)
{
    rasterizer_environment_diffuse_light_draw_function = rasterizer_environment_diffuse_light_draw_pp;
}
