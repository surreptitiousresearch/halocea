#include "../../headers/ws/ai/aiBODY.h"
#include "../../headers/ws/ai/aiWATCHER_PLAYER_fsm_boundary.h" // propFSM + fsmCB_HANDLER_dsSTRID + fsmCB_PARAM_BASE
#include "../../headers/ws/fsm/fsmAIMING_EVENT.h"
#include "../../headers/ws/m3d/m3dV.h"
#include "../../headers/ws/m3d/m3dMATR.h"
#include "../../headers/ws/m3d/m3d_boundary.h"                 // m3dVZero, m3dMatrIdentity
#include <string.h>

// aiBODY::SendFsmEventPosAim(const m3dV&) @ 0x832ACA80
// ?SendFsmEventPosAim@aiBODY@@UAA_NABUm3dV@@@Z
//
// Fire the aiming "set" callback with the given aim position; direction is zeroed, the transform is
// the identity matrix, and the trailing flags are cleared. Always returns true.
bool aiBODY::SendFsmEventPosAim(const m3dV &posAim)
{
    fsmAIMING_EVENT::SET_PARAM param;
    param.pos = posAim;
    param.dir = m3dVZero;
    memcpy(&param.matr, &m3dMatrIdentity, sizeof(param.matr));
    param.flag0 = false;
    param.flag1 = false;

    bodyFsm->eventHandler.ActivateCB(fsmAIMING_EVENT::set_(),
                                     reinterpret_cast<fsmCB_PARAM_BASE *>(&param));
    return true;
}
