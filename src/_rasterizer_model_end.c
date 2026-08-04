#include <stdint.h>
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_model_begin_parameters.h"
#include "headers/rasterizer_geometry_flags.h"
#include "headers/rasterizer_stencil_mode.h"
#include "headers/blam_data_globals.h"

extern void rasterizer_environment_fog_screen_model_end(void);
extern void rasterizer_set_stencil_mode(int16_t stencil_mode);
extern void rasterizer_set_frustum_z(float z_near, float z_far);

void _rasterizer_model_end(void)
{
    if (rasterizer_debug_options.draw_models)
    {
        if (local_environment_fog_screen_flag)
            rasterizer_environment_fog_screen_model_end();
        if ((local_parameters->geometry_flags & (1u << _rasterizer_geometry_first_person_bit)) != 0 && !local_do_not_change_z_stencil_states)
        {
            rasterizer_set_stencil_mode(_rasterizer_stencil_mode_reject);
            rasterizer_set_frustum_z(0.0f, 0.0f);
        }
        local_parameters = 0;
    }
}
