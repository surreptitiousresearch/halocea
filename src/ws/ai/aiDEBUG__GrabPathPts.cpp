// ws-engine ai08 (a8_debug.cpp): aiDEBUG::GrabPathPts @0x831944C0 (const).
// Copy the brain's current path points into `pathPts`. When the brain's status suppresses path
// display (general-state bit 2), clear the output. Otherwise, under the best-behaviour lock, find
// the active path (or zero-path) behaviour and dispatch its "grab path points" virtual.
#include "../../headers/ws/ai/aiDEBUG.h"
#include "../../headers/ws/ai/aiDEBUG_boundaries.h"          // aiBESTLock
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"                    // gen flags
#include "../../headers/ws/ai/aiBEST_BASE.h"                 // FindBHV_2
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h"  // AI_BID_PATH / AI_BID_ZERO_PATH
#include "../../headers/ws/ds/dsVECTOR.h"

void aiDEBUG::GrabPathPts(dsVECTOR<m3dV, 8> &pathPts, bool getOptimized) const
{
    aiSTATUS *status = pBrain->st.pointee;
    if (((static_cast<int>(status->gen.val) >> 2) & 1) != 0) {
        pathPts.nElem = 0;
        return;
    }

    aiBESTLock.Lock(nullptr, 0);

    aiBEST_BASE *best = pBrain->mind.best.pointee;
    void *bhv = best->FindBHV_2(AI_BID_PATH);
    if (!bhv)
        bhv = best->FindBHV_2(AI_BID_ZERO_PATH);

    if (bhv) {
        // dispatch the path behaviour's virtual "grab path points" slot (vtable byte offset 256)
        typedef void (*GrabPtsFn)(void *, dsVECTOR<m3dV, 8> *, int);
        GrabPtsFn grab = *reinterpret_cast<GrabPtsFn *>(*reinterpret_cast<char **>(bhv) + 256);
        grab(bhv, &pathPts, getOptimized);
    } else {
        pathPts.nElem = 0;
    }

    aiBESTLock.Unlock(nullptr, 0);
}
