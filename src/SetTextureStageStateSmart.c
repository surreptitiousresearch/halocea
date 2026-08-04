/* SetTextureStageStateSmart @0x8369B520 — empty stub in HCEX. In the original Xbox build this cached and
 * forwarded texture-stage-state changes to D3D; the fixed-function texture-stage pipeline does not exist on
 * the Xbox 360 D3D device, so the body was compiled out and every call site is a no-op. */

#include "headers/d3d_render_boundary.h"

void SetTextureStageStateSmart(unsigned int stage, _D3DTEXTURESTAGESTATETYPE State, unsigned int Value)
{
    (void)stage;
    (void)State;
    (void)Value;
}
