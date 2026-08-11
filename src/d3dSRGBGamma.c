#include "headers/d3d_render_state_boundary.h"
#include <math.h>

// @0x826950B0 -- standard sRGB OETF (linear-to-encoded) curve: a linear segment below the
// 0.0031308 knee, a power-law segment (gamma 1/2.4 == 0.41666666) above it.
float d3dSRGBGamma(float Clinear)
{
    if (Clinear > 0.0031308001)
        return (float)(pow(Clinear, 0.41666666) * 1.0549999 - 0.055);

    return (float)(Clinear * 12.92);
}
