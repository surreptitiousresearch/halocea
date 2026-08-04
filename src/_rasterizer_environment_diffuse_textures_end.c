/* _rasterizer_environment_diffuse_textures_end @0x8378A3C8 — restore the stencil mode after the environment
 * diffuse-texture pass. */

#include <stdint.h>
#include "headers/rasterizer_stencil_mode.h"

extern void rasterizer_set_stencil_mode(int16_t stencil_mode);

void _rasterizer_environment_diffuse_textures_end(void)
{
    rasterizer_set_stencil_mode(_rasterizer_stencil_mode_reject);
}
