#include "../../headers/ws/ai/aiBODY.h"
#include "../../headers/ws/ai/aiWATCHER_PLAYER_fsm_boundary.h" // propFSM + fsmSTATE pointees
#include "../../headers/ws/fsm/fsmEVENT_HANDLER.h"
#include "../../headers/ws/fsm/fsmMOVE_EVENT.h"
#include "../../headers/ws/m3d/m3dV.h"
#include "../../headers/ws/m3d/m3d_boundary.h"                 // m3dVUnitY

// aiBODY::SendFsmEventPosGoal(const m3dV&, bool) @ 0x832AC7F0
// ?SendFsmEventPosGoal@aiBODY@@UAA_NABUm3dV@@_N@Z
//
// Post a locomotion pos-goal ("set") event with the goal position and last-point flag; the goal
// facing defaults to +Y. Dispatched through the handler's virtual event+param slot (vtbl slot 5).
bool aiBODY::SendFsmEventPosGoal(const m3dV &posGoal, bool isLastPoint)
{
    fsmMOVE_EVENT::PARAM param;
    param.pos = posGoal;
    param.isLastPoint = isLastPoint;
    param.dir = m3dVUnitY;

    fsmEVENT_HANDLER *handler = bodyFsm->fsmHld.pointee
        ? static_cast<fsmEVENT_HANDLER *>(bodyFsm->fsmHld.pointee)
        : static_cast<fsmEVENT_HANDLER *>(bodyFsm->fsm.pointee);
    handler->PostEventByIdParam(&fsmMOVE_EVENT::set_(), &param);
    return true;
}
