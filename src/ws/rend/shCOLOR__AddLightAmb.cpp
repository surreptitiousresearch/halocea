#include <math.h>
#include "../../headers/ws/rend/shCOLOR.h"
#include "../../headers/ws/m3d/m3dCOLOR.h"
#include "../../headers/ws/m3d/m3d_boundary.h"

// shCOLOR::AddLightAmb @ 0x83095A00
//
// Add a flat ambient term to the SH band-0 (DC) coefficients of each colour channel.
//
// ABI: the mangled name (?AddLightAmb@shCOLOR@@QAAXMPAVm3dCOLOR@@@Z) has exactly two params —
// (float intensity, m3dCOLOR* color). The decompiler split the colour pointer into a phantom
// `color` plus the `a4` it actually reads; only the real colour pointer is kept here.
//
// `_coeffNorm` = 2*sqrt(pi), the SH DC normalisation; the binary computes it lazily behind a
// one-shot init flag — reproduced here as a function-local static.

void shCOLOR::AddLightAmb(float intensity, const m3dCOLOR *color)
{
    static bool  coeffNormInit = false;   // DB one-shot flag _S2_131 bit 0
    static float coeffNorm     = 0.0f;    // DB global _coeffNorm
    if (!coeffNormInit) {
        coeffNormInit = true;
        _m3dCheckValid(3.1415927f);
        coeffNorm = sqrtf(3.1415927f) * 2.0f;
    }

    shR.coeffs[0] += color->r * coeffNorm * intensity;
    shG.coeffs[0] += color->g * coeffNorm * intensity;
    shB.coeffs[0] += color->b * coeffNorm * intensity;
}
