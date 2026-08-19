/* _rasterizer_environment_shadow_model_begin @0x837A3A70 */
#include <stdint.h>
#include "headers/global_window_parameters.h"
#include "headers/rasterizer_globals.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_model_begin_parameters.h"
#include "headers/rasterizer_target.h"
#include "headers/blam_data_globals.h"

extern void rasterizer_set_model_skinning(const render_skinning *skinning, uint8_t uploadconstants);

void _rasterizer_environment_shadow_model_begin(const rasterizer_model_begin_parameters *parameters)
{
    if (global_window_parameters.rasterizer_target == _rasterizer_target_render_primary && !rasterizer_globals.render_targets_disabled)
    {
        if (rasterizer_debug_options.draw_environment_shadows)
        {
            /* DEVIATION: HIWORD = upper 16 bits; bit 8 of ~upper_16 = skinning type flag */
            unsigned int use_rigid = ((unsigned short)(~(parameters->geometry_flags >> 16)) >> 8) & 1;
            rasterizer_set_model_skinning(&parameters->skinning, use_rigid);
            shadow_used = 1;
            local_parameters_0 = parameters;
        }
    }
}
