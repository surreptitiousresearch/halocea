#include "../../headers/ws/ai/aiHO_SYS.h"
#include "../../headers/ws/ai/aiBRAIN.h"

// aiHO_SYS::EnterHO @ 0x832A7590
// ?EnterHO@aiHO_SYS@@UAAXV?$dsSHARED_PTR@VmdlITRC_BASE@@$0A@V?$Deleter@VmdlITRC_BASE@@@@@@@Z
//
// Adopt `itrc` as the current HO. When it references a live itrc and is not already the current one,
// leave the previous HO, notify the brain, and fire the enter event. `itrc` is consumed either way.
// (The decompiler materialises explicit shared-ptr copies for the by-value virtual calls; here the
// copies are implicit in the by-value argument passing.)
void aiHO_SYS::EnterHO(ItrcPtr itrc)
{
    if (itrc.pointee) {
        framesToLeaveHO = -1;
        if (!IsCurrentItrc(itrc)) {          // consumes a copy of itrc
            ProcessLeaveHO();
            pBrain->OnEnterHO();
            SendEnterHOEvent(itrc);          // consumes a copy of itrc
        }
    }
    itrc.Reset();
}
