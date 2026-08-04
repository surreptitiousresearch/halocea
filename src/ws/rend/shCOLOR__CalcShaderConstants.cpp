#include "../../headers/ws/rend/shCOLOR.h"
#include "../../headers/ws/rend/shSHD_COEFFS.h"

// shCOLOR::CalcShaderConstants @ 0x83095AB4
//
// Bake the 9 per-channel spherical-harmonics coefficients into the packed shader-constant
// block shSHD_COEFFS (three cA vectors carry bands 0/1 + part of band 2, three cB vectors carry
// the rest of band 2, and cC carries the m=0 band-2 term shared across channels). The scalar
// multipliers are the standard SH->linear-shader convolution constants; kept as literals.

void shCOLOR::CalcShaderConstants(shSHD_COEFFS *shdCoeffs)
{
    // cA: bands 0 and 1 (+ the l=2,m=0 term folded into .w).
    shdCoeffs->cAr.x = shR.coeffs[3] * -0.325735f;
    shdCoeffs->cAr.y = shR.coeffs[1] * -0.325735f;
    shdCoeffs->cAr.z = shR.coeffs[2] *  0.325735f;
    shdCoeffs->cAr.w = shR.coeffs[0] * 0.28209478f - shR.coeffs[6] * 0.078847893f;

    shdCoeffs->cAg.x = shG.coeffs[3] * -0.325735f;
    shdCoeffs->cAg.y = shG.coeffs[1] * -0.325735f;
    shdCoeffs->cAg.z = shG.coeffs[2] *  0.325735f;
    shdCoeffs->cAg.w = shG.coeffs[0] * 0.28209478f - shG.coeffs[6] * 0.078847893f;

    shdCoeffs->cAb.x = shB.coeffs[3] * -0.325735f;
    shdCoeffs->cAb.y = shB.coeffs[1] * -0.325735f;
    shdCoeffs->cAb.z = shB.coeffs[2] *  0.325735f;
    shdCoeffs->cAb.w = shB.coeffs[0] * 0.28209478f - shB.coeffs[6] * 0.078847893f;

    // cB: remaining band-2 terms (l=2, m=-2,-1,+1,+2).
    shdCoeffs->cBr.x = shR.coeffs[4] *  0.27313709f;
    shdCoeffs->cBr.y = shR.coeffs[5] * -0.27313709f;
    shdCoeffs->cBr.z = shR.coeffs[6] *  0.23654367f;
    shdCoeffs->cBr.w = shR.coeffs[7] * -0.27313709f;

    shdCoeffs->cBg.x = shG.coeffs[4] *  0.27313709f;
    shdCoeffs->cBg.y = shG.coeffs[5] * -0.27313709f;
    shdCoeffs->cBg.z = shG.coeffs[6] *  0.23654367f;
    shdCoeffs->cBg.w = shG.coeffs[7] * -0.27313709f;

    shdCoeffs->cC.w = 1.0f;

    shdCoeffs->cBb.x = shB.coeffs[4] *  0.27313709f;
    shdCoeffs->cBb.y = shB.coeffs[5] * -0.27313709f;
    shdCoeffs->cBb.z = shB.coeffs[6] *  0.23654367f;
    shdCoeffs->cBb.w = shB.coeffs[7] * -0.27313709f;

    // cC.xyz: the l=2,m=0 term (coeffs[8]) per channel.
    shdCoeffs->cC.x = shR.coeffs[8] * 0.13656855f;
    shdCoeffs->cC.y = shG.coeffs[8] * 0.13656855f;
    shdCoeffs->cC.z = shB.coeffs[8] * 0.13656855f;
}
