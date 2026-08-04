#include "../../headers/ws/ai/aiBODY.h"
#include "../../headers/ws/ai/aiWATCHER_PLAYER_fsm_boundary.h" // propFSM + fsmSTATE pointees
#include "../../headers/ws/fsm/fsmEVENT_HANDLER.h"
#include "../../headers/ws/fsm/fsmMOVE_EVENT.h"
#include "../../headers/ws/fsm/fsmHO_INFO.h"
#include "../../headers/ws/mdl/mdlITRC_BASE.h"                 // GetCornerPosType + ITRC_HO_POS_TYPE
#include "../../headers/ws/m3d/m3dRND_GEN.h"                   // m3dRndGen

// aiBODY::SetZeroGravity(bool) @ 0x832AD398
// ?SetZeroGravity@aiBODY@@UAAX_N@Z
//
// Entering zero-gravity: on a coin flip, if the body currently occupies a hideout corner, nudge the
// FSM up (left corner) or down (right corner) so it clears the geometry.
void aiBODY::SetZeroGravity(bool wasInHo)
{
    if (!wasInHo || m3dRndGen.RndFloat() <= 0.5f)
        return;

    fsmHO_INFO info; // shared-pointer members auto-release at scope end
    if (!GetFsmInfo(info) || !info.itrcHo.pointee)
        return;

    ITRC_HO_POS_TYPE corner = info.itrcHo.pointee->GetCornerPosType();
    if (corner != ITRC_HO_POS_LEFT && corner != ITRC_HO_POS_RIGHT)
        return;

    fsmEVENT_HANDLER *handler = bodyFsm->fsmHld.pointee
        ? static_cast<fsmEVENT_HANDLER *>(bodyFsm->fsmHld.pointee)
        : static_cast<fsmEVENT_HANDLER *>(bodyFsm->fsm.pointee);

    if (corner == ITRC_HO_POS_LEFT)
        handler->SendEvent<fsmMOVE_EVENT>(&fsmMOVE_EVENT::move_up);
    else
        handler->SendEvent<fsmMOVE_EVENT>(&fsmMOVE_EVENT::move_down);
}
