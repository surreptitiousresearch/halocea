/* rasterizer_models_initialize_pipeline @ 0x8378D218 */

#include "headers/rasterizer_draw_pipeline.h"

void rasterizer_models_initialize_pipeline(void)
{
    rasterizer_model_draw_environment_shader = rasterizer_model_draw_environment_shader_pp;
    rasterizer_model_draw_model_shader = rasterizer_model_draw_model_shader_pp;
}
