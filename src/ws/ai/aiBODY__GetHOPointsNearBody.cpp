#include "../../headers/ws/ai/aiBODY.h"
#include "../../headers/ws/ai/aiBRAIN.h"       // pBrain (aiBRAIN : aiBRAIN_IFACE : entENTITY)
#include "../../headers/ws/ai/aiWATCHER.h"     // IsPlayer / GetEntity
#include "../../headers/ws/gm/gmHIDEOUT_SYS.h" // gmHideOutSys + GetPointsNearBody
#include "../../headers/ws/mdl/mdlITRC_HO.h"

// 0x832ABAC0 ?GetHOPointsNearBody@aiBODY@@UBAX...
// Collect the hideout points near this body for a player target into `result`, filling `info` with
// the target/attacker entity+FSM. Non-player (or absent) targets yield an empty, all-null result.
void aiBODY::GetHOPointsNearBody(hoOBJECT *hideout, aiWATCHER *target, bool preferCenter,
    dsVECTOR<dsSHARED_PTR<mdlITRC_HO, 0, Deleter<mdlITRC_HO> >, 8> &result,
    aiBODY_IFACE::aiPTS_NEAR_BODY_ADDITIONAL_INFO &info) const
{
    info.entTarget = nullptr;
    info.entAttacker = nullptr;
    info.fsmTarget = nullptr;
    info.fsmAttacker = nullptr;

    if (!target || !target->IsPlayer())
        return;

    // The target watcher's associated body FSM sits at byte offset 0xB4 in the derived watcher
    // (past the 0x4C-byte aiWATCHER base); the decompiler expressed this as target[2].countRangedEnemies.
    propFSM *fsmTarget = *reinterpret_cast<propFSM *const *>(reinterpret_cast<const char *>(target) + 0xB4);

    gmHideOutSys->GetPointsNearBody(hideout, fsmTarget, this->bodyFsm, result, preferCenter);

    info.entTarget = target->GetEntity();
    info.entAttacker = static_cast<entENTITY *>(this->pBrain); // aiBRAIN's entENTITY base subobject
    info.fsmTarget = fsmTarget;
    info.fsmAttacker = this->bodyFsm;
}
