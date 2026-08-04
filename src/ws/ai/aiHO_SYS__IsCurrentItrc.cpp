#include "../../headers/ws/ai/aiHO_SYS.h"
#include "../../headers/ws/mdl/mdlITRC_BASE.h"  // ItrcPtr

// aiHO_SYS::IsCurrentItrc @ 0x832A7E78
// ?IsCurrentItrc@aiHO_SYS@@MAA_NV?$dsSHARED_PTR@...@@@Z
//
// True when `itrc` is the hideout ITRC the body currently holds. Consumes `itrc`.
bool aiHO_SYS::IsCurrentItrc(ItrcPtr itrc)
{
    ItrcPtr subject = itrc;              // copy compared against the current ITRC
    ItrcPtr current = GetCurrentItrc();
    bool same = IsSameItrc(current, subject); // consumes both copies
    itrc.Reset();
    return same;
}
