#include "../../headers/ws/ai/aiBRAIN.h"     // pulls aiCMD2BODY + aiBRAIN (pBrain->spDesc)
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/m3d/m3dV.h"

extern const m3dV m3dVZero; // ?m3dVZero@@3Um3dV@@A — global zero vector (boundary)

// aiCMD2BODY::FillDefault @ 0x83178870
// ?FillDefault@aiCMD2BODY@@QAAXXZ
//
// Reset the command block to defaults: look straight ahead, zero the goal position/reach point,
// use a 0.5 base reach radius, and seed the reach multiplier from the brain's descriptor.
void aiCMD2BODY::FillDefault()
{
    dscDESC *pDesc = pBrain->spDesc.pointee;

    LookStraight();

    goal.posGoal = m3dVZero;
    goal.posChkReach = m3dVZero;
    goal.distReach = 0.5f;

    // Reach multiplier lives in the derived (brain) descriptor, past dscDESC's own fields — the
    // descriptor subclass layout is the next frontier, so read it by verified byte offset (0x2CC).
    unsigned char *descBytes = reinterpret_cast<unsigned char *>(pDesc);
    goal.distReachMultiplier = *reinterpret_cast<float *>(descBytes + 0x2CC);
}
