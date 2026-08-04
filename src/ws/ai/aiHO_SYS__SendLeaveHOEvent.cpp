#include "../../headers/ws/ai/aiHO_SYS.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBODY_IFACE.h"
#include "../../headers/ws/ai/aiDEBUG.h"
#include "../../headers/ws/fsm/fsmHIDEOUT_EVENT.h"

// aiHO_SYS::SendLeaveHOEvent @ 0x832A67F8
// ?SendLeaveHOEvent@aiHO_SYS@@MAAXXZ
//
// Fire the body FSM's "leave hideout" event and note the transition in the brain's mini-log.
void aiHO_SYS::SendLeaveHOEvent()
{
    aiBODY_IFACE *body = pBrain->body.pointee;
    body->SendFsmEventByName_2(&fsmHIDEOUT_EVENT::leave_());
    pBrain->dbg.minilogEnterLeaveHO = "left";
}
