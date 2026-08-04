/* rasterizer_environment_diffuse_textures_initialize_pipeline @ 0x8378A3D0 */

#include "headers/rasterizer_draw_pipeline.h"

void rasterizer_environment_diffuse_textures_initialize_pipeline(void)
{
    rasterizer_environment_diffuse_texture_draw_function = rasterizer_environment_diffuse_texture_draw_pp;
}
