#include "headers/d3d_render_state_boundary.h"

// 0x82696538
float d3dDRIVER::ConvertPWL2ProperGamma(double C) const
{
    if (dbg_correctPWLGamma.value && this->enablePWLGamma)
        return d3dSRGBGamma(d3dPWLDeGamma(C));

    return (float)C;
}
