// ws-engine ai08 (a8_debug.cpp): aiDEBUG::GrabHOPathPts @0x831945E0 (const).
// Copy the brain's high-obstacle path points into `pathPts` (reporting start/end scalars). Under
// the best-behaviour lock, find the HO-path behaviour (aiBID 6) and forward to its exporter; if
// absent, clear the output.
#include "../../headers/ws/ai/aiDEBUG.h"
#include "../../headers/ws/ai/aiDEBUG_boundaries.h"   // aiBESTLock / bhvHO_PATH
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBEST_BASE.h"          // FindBHV_2
#include "../../headers/ws/ds/dsVECTOR.h"

void aiDEBUG::GrabHOPathPts(dsVECTOR<m3dV, 8> &pathPts, float *posStart, float *posEnd) const
{
    aiBESTLock.Lock(nullptr, 0);

    // aiBID 6 = the high-obstacle path behaviour.
    bhvHO_PATH *ho = reinterpret_cast<bhvHO_PATH *>(pBrain->mind.best.pointee->FindBHV_2(6));
    if (ho) {
        ho->GrabHOPathPts(pathPts, posStart, posEnd);
    } else {
        pathPts.nElem = 0;
    }

    aiBESTLock.Unlock(nullptr, 0);
}
