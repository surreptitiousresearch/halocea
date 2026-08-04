#include "../../headers/ws/ai/aiLOD_SYS.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiWAH_SYS.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h"      // aiPLANNER *aiPlanner
#include "../../headers/ws/ai/aiDEBUG_boundaries.h"    // rendDrv, dbg_is*, m3dVUnitX, operators, m3dNormalize
#include "../../headers/ws/m3d/m3d_boundary.h"         // m3dVUnitZ
#include "../../headers/ws/msg/msgSYSTEM.h"            // msgSystem
#include "../../headers/ws/msg/msgTYPE_ITER.h"         // msgTYPE_ITER<aiBRAIN>
#include "../../headers/ws/rend/rendDRIVER.h"          // rendDrv->RenderLine
#include "../../headers/ws/ds/WEAK_PTR.h"
#include <math.h>

// aiLOD_SYS::DebugRender() @ 0x83239458
// ?DebugRender@aiLOD_SYS@@QBAXXZ
//
// Debug overlay for the AI level-of-detail subsystem. Two independent, con-var-gated layers:
//   dbg_isLODsValues       — for every active AI unit, draw a vertical "bar" above the body: one
//                            up-axis segment per LOD level (the last level scaled by its fractional
//                            remainder) with a small tick at each level and a cap crossbar top/bottom.
//   dbg_isRenderMindSharing (unless dbg_isOffMindSharing) — for the single brain the mind-sparser
//                            currently permits (msAllowed.pb), draw a spoked 0.3-radius ring above
//                            its head marking it as the mind-frame owner this tick.
//
// DEVIATION: the decompiler lost several operand vectors here (they were re-lowered through reused
// r3/r4 return registers). The stroke counts, scalar factors and colours below are exact; the axis
// of every crossbar/tick and the ring's plane basis are reconstructed structurally (m3dVUnitX /
// m3dVUnitZ world axes) — this is a debug marker, not a binary-fidelity-critical layout.
void aiLOD_SYS::DebugRender() const
{
    aiPLANNER *planner = aiPlanner;
    if (!planner)
        return;

    if (dbg_isLODsValues.value)
    {
        int nUnits = planner->wah.arrUnits.nElem;
        for (int i = 0; i < nUnits; ++i)
        {
            ds::WEAK_PTR_HANDLE<aiBRAIN> *handle = planner->wah.PeekActive(i)->brain.pHandle;
            aiBRAIN *brain = handle ? handle->pPtr : nullptr;
            aiSTATUS *st = brain->st.pointee;

            m3dV pos = st->cs.posBody;
            m3dV up  = st->cs.dirUpBody;
            m3dNormalize(&up);
            pos += 2.0f * up;

            // bottom cap crossbar (half-extent 1.0 along the reconstructed world-X axis).
            m3dV cap = 1.0f * m3dVUnitX;
            m3dV from = pos - cap;
            m3dV to   = pos + cap;
            rendDrv->RenderLine(&from, &to, 0xFF00FF00);

            for (int value = st->lod.value; value > 0; )
            {
                float seg = (value <= 1) ? (float)value : 1.0f;
                --value;

                // vertical LOD segment.
                m3dV segFrom = pos;
                m3dV segTo   = pos + up * seg;
                rendDrv->RenderLine(&segFrom, &segTo, 0xFF00FF00);
                pos += up * seg;

                // level tick (half-width 0.5 along world-X).
                m3dV tick     = m3dVUnitX * 0.5f;
                m3dV tickFrom = pos - tick;
                m3dV tickTo   = pos + tick;
                rendDrv->RenderLine(&tickFrom, &tickTo, 0xFF00FF00);
            }

            // top cap crossbar.
            m3dV topFrom = pos - cap;
            m3dV topTo   = pos + cap;
            rendDrv->RenderLine(&topFrom, &topTo, 0xFF00FF00);
        }
    }

    if (dbg_isRenderMindSharing.value && !dbg_isOffMindSharing.value && msAllowed.pb)
    {
        for (msgTYPE_ITER<aiBRAIN> it(msgSystem.Front()); !it.IsDone(); ++it)
        {
            const aiBRAIN *brain = it.Get();
            if (brain != msAllowed.pb)
                continue;

            aiSTATUS *st = brain->st.pointee;
            if (((unsigned int)st->gen.val >> 4) & 1)
            {
                m3dV pos = st->cs.posBody;
                m3dV up  = st->cs.dirUpBody;
                m3dNormalize(&up);
                pos += 2.0f * up;

                m3dV center = pos + 0.1f * up;                 // spoke hub, slightly above the ring
                m3dV start  = pos + m3dVUnitX * 0.3f;          // ring point at angle 0
                m3dV prev   = start;
                for (float ang = 31.0f; ang < 360.0f; ang += 31.0f)
                {
                    float rad = ang * 0.017453292f;
                    m3dV pt = pos + m3dVUnitX * ((float)sin(rad) * 0.3f)
                                  + m3dVUnitZ * ((float)cos(rad) * 0.3f);
                    rendDrv->RenderLine(&prev, &pt, 0xFF7777EE);
                    rendDrv->RenderLine(&prev, &center, 0xFF7777EE);
                    prev = pt;
                }
                rendDrv->RenderLine(&prev, &start, 0xFF7777EE);
            }
        }
    }
}
