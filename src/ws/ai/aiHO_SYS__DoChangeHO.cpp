#include "../../headers/ws/ai/aiHO_SYS.h"
#include "../../headers/ws/ai/aiBRAIN.h"       // pBrain->body
#include "../../headers/ws/ai/aiBODY_IFACE.h"  // SendFsmEventByName
#include "../../headers/ws/fsm/fsmHIDEOUT_EVENT.h"
#include "../../headers/ws/mdl/mdlITRC_BASE.h"

// aiHO_SYS::DoChangeHO @ 0x832A7EF0
// ?DoChangeHO@aiHO_SYS@@UAA?AV?$dsSHARED_PTR@...@@PAVmdlITRC_HO@@ABW4HO_CHANGE_TYPE@@@Z
//
// Adopt `itrc` as the new hideout ITRC (returned by value) and fire the body FSM's "change_side"
// hideout event carrying it. (The decompiler swaps the hidden sret buffer with `this`; disasm confirms
// r5=itrc is adopted into the return handle, r4(this)+4=pBrain, pBrain+0x1C0=body.pointee, and the
// body's SendFsmEventByName — vtbl slot 23 — is dispatched. `changeType` is not read on this path.)
aiHO_SYS::ItrcPtr aiHO_SYS::DoChangeHO(mdlITRC_HO *itrc, const HO_CHANGE_TYPE &changeType)
{
    (void)changeType; // this build's change-side path does not consult the change type

    ItrcPtr adopted(reinterpret_cast<mdlITRC_BASE *>(itrc));

    aiBODY_IFACE *bodyIface = pBrain->body.pointee;
    fsmHIDEOUT_EVENT::PARAM param(adopted); // records the adopted itrc as the event's previous ITRC
    bodyIface->SendFsmEventByName(&fsmHIDEOUT_EVENT::change_side_(), &param);

    return adopted;
}
