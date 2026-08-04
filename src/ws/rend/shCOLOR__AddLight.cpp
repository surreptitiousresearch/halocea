#include "../../headers/ws/rend/shCOLOR.h"
#include "../../headers/ws/m3d/m3dV.h"
#include "../../headers/ws/m3d/m3dCOLOR.h"

// shCOLOR::AddLight @ 0x83095698  (?AddLight@shCOLOR@@QAAXPAUm3dV@@PAVm3dCOLOR@@@Z)
//
// Project a directional light of colour `color` arriving from unit direction `dir` onto the
// 9-coefficient (bands 0..2) SH basis and accumulate it into each colour channel.
//
// The five per-band SH evaluation constants (fC0..fC4) are the standard real-SH basis scalars
// times the shared `normalization` factor (2.9567931). The binary computes each lazily behind a
// bit in the one-shot flag _S1_230; reproduced here as function-local statics.

void shCOLOR::AddLight(const m3dV *dir, const m3dCOLOR *color)
{
    static bool  init = false;   // DB one-shot flag _S1_230 bits 0..5
    static float normalization;  // DB global `normalization`
    static float fC0, fC1, fC2, fC3, fC4;
    if (!init) {
        init = true;
        normalization = 2.9567931f;
        fC0 = normalization * 0.28209499f; // band 0 (Y0,0)
        fC1 = normalization * 0.488603f;   // band 1 (Y1,*)
        fC2 = normalization * 1.092548f;   // band 2, m = -2,-1,+1
        fC3 = normalization * 0.31539199f; // band 2, m = 0
        fC4 = normalization * 0.54627401f; // band 2, m = +2
    }

    const float x = dir->x, y = dir->y, z = dir->z;

    // Per-basis directional response (shared across the three colour channels).
    const float b0 = fC0;
    const float b1y = -(y * fC1);
    const float b1z =   z * fC1;
    const float b1x = -(x * fC1);
    const float b2_xy =   x * y * fC2;
    const float b2_zy = -(z * y * fC2);
    const float b2_z2 = (3.0f * z * z - 1.0f) * fC3;
    const float b2_xz = -(x * z * fC2);
    const float b2_x2y2 = (x * x - y * y) * fC4;

    shR.coeffs[0] += color->r * b0;
    shG.coeffs[0] += color->g * b0;
    shB.coeffs[0] += color->b * b0;

    shR.coeffs[1] += color->r * b1y;
    shG.coeffs[1] += color->g * b1y;
    shB.coeffs[1] += color->b * b1y;

    shR.coeffs[2] += color->r * b1z;
    shG.coeffs[2] += color->g * b1z;
    shB.coeffs[2] += color->b * b1z;

    shR.coeffs[3] += color->r * b1x;
    shG.coeffs[3] += color->g * b1x;
    shB.coeffs[3] += color->b * b1x;

    shR.coeffs[4] += color->r * b2_xy;
    shG.coeffs[4] += color->g * b2_xy;
    shB.coeffs[4] += color->b * b2_xy;

    shR.coeffs[5] += color->r * b2_zy;
    shG.coeffs[5] += color->g * b2_zy;
    shB.coeffs[5] += color->b * b2_zy;

    shR.coeffs[6] += color->r * b2_z2;
    shG.coeffs[6] += color->g * b2_z2;
    shB.coeffs[6] += color->b * b2_z2;

    shR.coeffs[7] += color->r * b2_xz;
    shG.coeffs[7] += color->g * b2_xz;
    shB.coeffs[7] += color->b * b2_xz;

    shR.coeffs[8] += color->r * b2_x2y2;
    shG.coeffs[8] += color->g * b2_x2y2;
    shB.coeffs[8] += color->b * b2_x2y2;
}
