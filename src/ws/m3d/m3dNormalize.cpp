#include <math.h>
#include "m3d_boundary.h"

// m3dNormalize(m3dV*) @ 0x823CD320
//
// CAVEAT: the shipped routine is dense VMX128 (lvlx128/lvrx128 load, vmsum3fp128 dot
// product, vrsqrtefp128 reciprocal-sqrt estimate + one Newton-Raphson refinement step,
// vcmpeqfp128/vsel to special-case the zero vector). Modelled here as scalar equivalent
// arithmetic -- sqrtf(0) guard reproduces the zero-vector special case, which the VMX
// vsel resolves by substituting m3dVUnitY when the squared length compares equal to zero
// (matching the established precedent in m3dMATR::GetScale for the same VMX rsqrt idiom).
void m3dNormalize(m3dV *v)
{
    _m3dCheckValid(v);

    float lengthSq = v->x * v->x + v->y * v->y + v->z * v->z;

    if (lengthSq == 0.0f)
    {
        *v = m3dVUnitY;
    }
    else
    {
        float invLength = 1.0f / sqrtf(lengthSq);
        v->x *= invLength;
        v->y *= invLength;
        v->z *= invLength;
    }
}
