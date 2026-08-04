#include "../../headers/ws/ai/aiHO_SYS.h"
#include "../../headers/ws/mdl/mdlITRC_BASE.h"  // dsSHARED_PTR<mdlITRC_BASE>::pointee

// aiHO_SYS::IsInHO @ 0x832A76A8
// ?IsInHO@aiHO_SYS@@UAA_NXZ
//
// True when the body currently holds a hideout ITRC. GetCurrentItrcHO returns an empty handle when
// the current ITRC is not an HO, so a non-null pointee is the whole test.
bool aiHO_SYS::IsInHO()
{
    ItrcPtr itrc = GetCurrentItrcHO();
    return itrc.pointee != nullptr;
}
