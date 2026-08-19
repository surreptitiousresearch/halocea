/* _rasterizer_environment_shadow_model_end @0x837A3DC0 */
/* _rasterizer_environment_shadow_model_end 0x837A3DC0 — end a shadow-casting model's contribution to the
 * environment shadow pass by clearing the cached submit parameters set in _rasterizer_environment_shadow_model_begin. */

#include "headers/rasterizer_model_begin_parameters.h"
#include "headers/blam_data_globals.h"


void _rasterizer_environment_shadow_model_end(void)
{
    local_parameters_0 = nullptr;
}
