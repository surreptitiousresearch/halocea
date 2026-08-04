/* _rasterizer_environment_reflection_lightmap_masks_end @0x837A6A08 — end the reflection lightmap-mask pass
 * by clearing the cached DX effect shader. */

#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/blam_data_globals.h"


void _rasterizer_environment_reflection_lightmap_masks_end(void)
{
    dxeffect_shader = nullptr;
}
