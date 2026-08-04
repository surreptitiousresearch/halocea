#include "../../headers/ws/ai/aiHO_SYS.h"
#include "../../headers/ws/ai/aiBRAIN.h"       // pBrain->OnLeaveHO
#include "../../headers/ws/mdl/mdlITRC_BASE.h"  // ItrcPtr::pointee

// aiHO_SYS::ProcessLeaveHO @ 0x832A7B08
// ?ProcessLeaveHO@aiHO_SYS@@MAAXXZ
//
// If the body is still in a hideout, send the leave event to the body FSM and tell the brain it left.
void aiHO_SYS::ProcessLeaveHO()
{
    ItrcPtr current = GetCurrentItrc();
    bool inHO = current.pointee != nullptr;
    current.Reset();

    if (inHO) {
        SendLeaveHOEvent();
        pBrain->OnLeaveHO();
    }
}
