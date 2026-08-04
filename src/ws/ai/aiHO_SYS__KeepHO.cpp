#include "../../headers/ws/ai/aiHO_SYS.h"
#include "../../headers/ws/mdl/mdlITRC_BASE.h"  // ItrcPtr::pointee

// aiHO_SYS::KeepHO @ 0x832A7950
// ?KeepHO@aiHO_SYS@@UAAXXZ
//
// While still holding a hideout ITRC, cancel any pending "leave" by resetting the leave countdown to
// its idle sentinel (-1).
void aiHO_SYS::KeepHO()
{
    ItrcPtr current = GetCurrentItrc();
    bool inHO = current.pointee != nullptr;
    current.Reset();
    if (inHO)
        framesToLeaveHO = -1;
}
