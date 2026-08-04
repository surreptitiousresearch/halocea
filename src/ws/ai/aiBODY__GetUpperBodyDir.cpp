#include "../../headers/ws/ai/aiBODY.h"
#include "../../headers/ws/ai/aiWATCHER_PLAYER_fsm_boundary.h" // propFSM + fsmCB_HANDLER_dsSTRID + fsmCB_PARAM_BASE
#include "../../headers/ws/fsm/fsmBODY_FSM_EVENT.h"
#include "../../headers/ws/m3d/m3dV.h"

// aiBODY::GetUpperBodyDir(m3dV*) const @ 0x832AC978
// ?GetUpperBodyDir@aiBODY@@UBA_NPAUm3dV@@@Z
//
// Query the body FSM's "get_cur_aiming" callback, which writes the current upper-body aiming
// direction into the head of the callback param block; copy it into `dir`. False when `dir` is null.
bool aiBODY::GetUpperBodyDir(m3dV *dir) const
{
    if (!dir)
        return false;

    // The callback fills a param block whose leading m3dV is the aiming direction (24-byte block
    // observed at the call site; only the first vector is consumed here).
    unsigned char paramBlock[24];
    bodyFsm->eventHandler.ActivateCB(fsmBODY_FSM_EVENT::get_cur_aiming_(),
                                     reinterpret_cast<fsmCB_PARAM_BASE *>(paramBlock));

    const float *aiming = reinterpret_cast<const float *>(paramBlock);
    dir->x = aiming[0];
    dir->y = aiming[1];
    dir->z = aiming[2];
    return true;
}
