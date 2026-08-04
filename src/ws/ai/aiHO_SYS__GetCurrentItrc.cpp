#include "../../headers/ws/ai/aiHO_SYS.h"
#include "../../headers/ws/ai/aiBRAIN.h"       // pBrain->body
#include "../../headers/ws/ai/aiBODY_IFACE.h"  // aiBODY_IFACE::GetCurrentItrcHO
#include "../../headers/ws/mdl/mdlITRC_BASE.h"  // mdlITRC_BASE::IsHO

// aiHO_SYS::GetCurrentItrc @ 0x832A77E8
// ?GetCurrentItrc@aiHO_SYS@@IAA?AV?$dsSHARED_PTR@VmdlITRC_BASE@@$0A@V?$Deleter@VmdlITRC_BASE@@@@@@XZ
//
// Ask the body for its current ITRC; keep it only if it is actually a hideout ITRC, otherwise return
// an empty handle. (The decompiler swaps the hidden sret buffer with `this`; disasm confirms r4 is the
// real aiHO_SYS and reads pBrain at +4, then body at aiBRAIN+0x1C0 = body.pointee, and dispatches the
// body's GetCurrentItrcHO — vtbl slot 36 — with its enum selector = 0.)
aiHO_SYS::ItrcPtr aiHO_SYS::GetCurrentItrc()
{
    ItrcPtr current; // empty handle

    if (pBrain) {
        ItrcPtr itrc = pBrain->body.pointee->GetCurrentItrcHO();
        if (itrc.pointee && itrc.pointee->IsHO())
            current = itrc;
    }
    return current;
}
