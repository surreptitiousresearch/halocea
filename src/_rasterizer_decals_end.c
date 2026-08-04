/* _rasterizer_decals_end @0x83711518 — end the decal render pass: clear the depth-bias state and, if the
 * active decal layer is the topmost (3), restore the stencil mode to 2. */

#include <stdint.h>
#include "headers/decal_layer.h"
#include "headers/rasterizer_stencil_mode.h"
#include "headers/blam_data_globals.h"

extern void rasterizer_dx9_reset_zbias(void);
extern void rasterizer_set_stencil_mode(int16_t stencil_mode);

void _rasterizer_decals_end(void)
{
    rasterizer_dx9_reset_zbias();
    if (local_layer == _decal_layer_alpha_tested)
        rasterizer_set_stencil_mode(_rasterizer_stencil_mode_reject);
}
