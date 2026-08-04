#include "../../headers/ws/ai/aiVISION_COMMON.h"
#include "../../headers/ws/ai/aiBRAIN.h"                    // pBrain->dbg / st.pointee / Dist
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"            // peekerSelf->IsHideoutMode
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h" // m3dIsBelongPointDblCone
#include "../../headers/ws/m3d/m3dV.h"

// aiVISION_COMMON::IsPotentiallyVisible @ 0x832914E8
// ?IsPotentiallyVisible@aiVISION_COMMON@@UBA_NABUm3dV@@0@Z
// Virtual const: cheap pre-visibility test. Records the eye / target check points for debug, then
// rejects the target if it is beyond the straight sight radius. Otherwise the target is potentially
// visible when its watcher is in hideout mode (or has no watcher), or when the check point lies
// inside the viewer's double sight cone.
bool aiVISION_COMMON::IsPotentiallyVisible(const m3dV &posEye, const m3dV &posCheck) const
{
    pBrain->dbg.posEye = posEye;
    pBrain->dbg.posEnemyVisCheck = posCheck;

    aiBRAIN *brain = pBrain;
    aiSTATUS *st = brain->st.pointee;

    if (brain->Dist(&posCheck) > (double)st->perc.senses.radSeeStraight)
        return false;

    aiWATCHER_BRAIN *peeker = st->peekerSelf;
    bool skipCone = peeker ? peeker->IsHideoutMode() : true;

    return skipCone
        || m3dIsBelongPointDblCone(&posCheck, &posEye, &dirSight,
                                   st->perc.senses.radSeeStraight, st->perc.senses.radSeeSide,
                                   st->perc.senses.angleSeeStraight, st->perc.senses.angleSeeSide) != 0;
}
