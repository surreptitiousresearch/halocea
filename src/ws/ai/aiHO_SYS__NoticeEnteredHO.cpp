#include "../../headers/ws/ai/aiHO_SYS.h"
#include "../../headers/ws/ai/aiBRAIN.h"       // pBrain->OnEnterHO
#include "../../headers/ws/mdl/mdlITRC_BASE.h"  // ItrcPtr::pointee

// aiHO_SYS::NoticeEnteredHO @ 0x832A7638
// ?NoticeEnteredHO@aiHO_SYS@@UAAXV?$dsSHARED_PTR@...@@@Z
//
// Record that the body just entered the hideout referenced by `itrc`: reset the leave countdown and,
// unless this is already the current ITRC, notify the owning brain (OnEnterHO). Consumes `itrc`.
void aiHO_SYS::NoticeEnteredHO(ItrcPtr itrc)
{
    if (itrc.pointee) {
        framesToLeaveHO = -1;
        // IsCurrentItrc consumes the copy it is given; the original `itrc` is released below.
        if (!IsCurrentItrc(itrc))
            pBrain->OnEnterHO();
    }
    itrc.Reset();
}
