#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiENEMY_BASE.h"
#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/m3d/m3d_boundary.h"

// aiBRAIN::implCalcReticleDanger() @ 0x832491F0
// ?implCalcReticleDanger@aiBRAIN@@UAAMXZ
//
// Virtual: reticle-based danger heuristic. Zero when there is no tracked enemy or the enemy has no
// peeker (line-of-fire) watcher. Otherwise:
//   +40 when the enemy's peeker is actively shooting and the reticle is close (< 5 units), and
//   + a distance-ramped term (60 at reticle-dist 1.5, falling to 0 at 4.0) while in mini-zoom mode.
float aiBRAIN::implCalcReticleDanger()
{
    if (!mind.enemy.pointee)
        return 0.0f;

    aiWATCHER *peeker = mind.enemy.pointee->GetPeeker();
    if (!peeker)
        return 0.0f;

    aiSTATUS *status = st.pointee;
    float danger = 0.0f;

    if (peeker->IsShooting() && status->perc.distToReticle < 5.0f)
        danger = 40.0f;

    if (peeker->IsMiniZoomMode())
        danger += m3dClamp(60.0f, 0.0f, 1.5f, 4.0f, status->perc.distToReticle);

    return danger;
}
