// aiDEBUG::RenderSight @ 0x8319AEF0
#include "../../headers/ws/ai/aiDEBUG.h"
#include "../../headers/ws/ai/aiDEBUG_render_boundaries.h" // rendDrv, RenderPolyline, dirVLength,
                                                           // m3dVUnitZNeg, dbg_isSightConeSimple, EPS
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/m3d/m3d_boundary.h"            // m3dLengthVector(_2), _m3dNormalize,
                                                           // _m3dCheckValid, m3dCrossProduct
#include "../../headers/ws/rend/rendDRIVER.h"
#include <math.h>

// Draw the brain's sight-line plus either a simple flat sight polygon or the full vision cone,
// depending on the dbg_isSightConeSimple con-var.
void aiDEBUG::RenderSight()
{
    // Use the recorded eye position, falling back to the body position when the eye is unset.
    m3dV startPos;
    if (fabsf(m3dLengthVector_2(&this->posEye)) < M3D_EPSILON_2_544)
        startPos = this->pBrain->st.pointee->cs.posBody;
    else
        startPos = this->posEye;

    m3dV sightDir = this->dirSight;
    _m3dNormalize(&sightDir);
    if (m3dLengthVector(&sightDir) >= 0.000001f)
    {
        m3dV lineVec = sightDir * dirVLength;
        _m3dCheckValid(&startPos);
        _m3dCheckValid(&lineVec);
        m3dV endPos = lineVec + startPos;
        rendDrv->RenderLine(&startPos, &endPos, 0xFFEE0000);

        // Right vector of the cone = normalize(sightDir x bodyUp).
        m3dV rightDir = m3dVUnitZNeg;
        m3dV upBody = this->pBrain->st.pointee->cs.dirUpBody;
        m3dCrossProduct(&sightDir, &upBody, &rightDir);
        _m3dNormalize(&rightDir);
        m3dV rightVec = rightDir * dirVLength;
        _m3dCheckValid(&startPos);
        _m3dCheckValid(&rightVec);

        // color arg was lost by the decompiler; disasm shows r6 = 0xFF00EE00.
        rendDrv->RenderCross(&this->posEnemyVisCheck, 0.40000001f, 0xFF00EE00);

        if (dbg_isSightConeSimple.value)
        {
            static dsVECTOR<m3dV, 8> shape;
            GetSightShape(startPos, sightDir, rightDir, shape);
            RenderPolyline(&shape, 0xFFEE77EE);
        }
        else
        {
            static dsVECTOR<dsVECTOR<m3dV, 8>, 8> visCone;
            CreateVisionCone(&startPos, &sightDir, &rightDir, &visCone);
            for (int i = 0; i < visCone.nElem; ++i)
                RenderPolyline(&visCone[i], 0xFFEE77EE);
        }
    }
}
