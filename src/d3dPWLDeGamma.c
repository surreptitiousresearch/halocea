#include "headers/d3d_render_state_boundary.h"

// 0x82695154 -- Xbox 360 EDRAM piecewise-linear gamma-decode curve: `C` (0..1) is scaled to an
// 8-bit channel value, then mapped through 4 linear segments (breakpoints at 64/96/192) each
// normalized by 1/1023 (0.00097751711 == 1/1023.0).
float d3dPWLDeGamma(float C)
{
    double channel8 = C * 255.0;

    if (channel8 >= 192.0)
        return (float)(((channel8 - 192.0) * 8.0 + 513.0) * 0.00097751711);
    if (channel8 >= 96.0)
        return (float)(((channel8 - 96.0) * 4.0 + 128.0) * 0.00097751711);
    if (channel8 >= 64.0)
        return (float)(((channel8 - 64.0) * 2.0 + 64.0) * 0.00097751711);

    return (float)(channel8 * 0.00097751711);
}
