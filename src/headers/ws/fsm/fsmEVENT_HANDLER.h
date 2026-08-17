#pragma once
#include "../ds/dsSTRID.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsCONST_ARRAY.h"
#include "../ds/dsSHARED_PTR.h"
#include "fsmCB_HANDLER_T.h"
// ws-engine fsm — the event handler owned by a body FSM state object (fsmSTATE_MANAGER /
// fsmSTATE_FSM_HLD both derive it). Only the entry points the aiBODY event-forwarding methods reach
// are modelled; the full layout/vtable is the next frontier.  boundary.

struct fsmEVENT_PARAM_BASE; // fsm/fsmAIMING_EVENT.h — empty base of every FSM event param block
struct fsmEVENT_HANDLER_vtbl; // DB size 88 (22 slots) — dispatch table, slots reached by index below
struct fsmEVENT_HANDLER_DESC; // DB size 4 {__vftable@0} — the handler's descriptor; pointer only

// ?...@fsmEVENT_HANDLER@@... — the concrete handler. Virtual slots reached by aiBODY are dispatched
// by their DB-verified index (like aiBODY_IFACE), without fabricating slot names.
// DB-verified layout (types_members fsmEVENT_HANDLER, types size 12): __vftable@0
// (fsmEVENT_HANDLER_vtbl *), desc@4 (fsmEVENT_HANDLER_DESC *), owner@8 (fsmEVENT_HANDLER *).
struct fsmEVENT_HANDLER {
    fsmEVENT_HANDLER_vtbl *__vftable; // 0x00
    fsmEVENT_HANDLER_DESC *desc;      // 0x04 descriptor this handler was created from
    fsmEVENT_HANDLER      *owner;     // 0x08 handler this one forwards unhandled events to

    // vtbl slot 1 — virtual ?SendEventByName@fsmEVENT_HANDLER@@UAAXABVdsSTRID@@@Z: post the named
    // event with no parameter. DB-verified slot index (site: aiBODY::SendFsmEventByName 1-arg).
    void PostEventById(const dsSTRID *ev) {
        typedef void (*Fn)(fsmEVENT_HANDLER *, const dsSTRID *);
        (reinterpret_cast<Fn *>(__vftable)[1])(this, ev);
    }
    // vtbl slot 5 — virtual: post the named event with a raw parameter block (site:
    // aiBODY::SendFsmEventPosGoal, dispatched at *(__vftable)+20). DB-verified slot index.
    void PostEventByIdParam(const dsSTRID *ev, void *param) {
        typedef void (*Fn)(fsmEVENT_HANDLER *, const dsSTRID *, void *);
        (reinterpret_cast<Fn *>(__vftable)[5])(this, ev, param);
    }

    // ?SendEventByName@fsmEVENT_HANDLER@@QAAXABVdsSTRID@@ABUfsmEVENT_PARAM_BASE@@@Z @ 0x82CF8EF8 —
    // non-virtual: post the named event carrying `par`. boundary.
    void SendEventByName(const dsSTRID &ev, const fsmEVENT_PARAM_BASE &par);

    // ??$SendEvent@V<EV>@@@fsmEVENT_HANDLER@@QAAX P8<EV>@@AAABVdsSTRID@@XZ @Z — non-virtual template:
    // post the event whose interned id is produced by the member id-getter `idGetter` of event
    // class EV (no parameter overload). boundary. (site: aiBODY::SetZeroGravity, EV=fsmMOVE_EVENT).
    template<class EV>
    void SendEvent(const dsSTRID &(EV::*idGetter)());
};

// The body-FSM state-object family. Kept in this header (the family's canonical home — existing
// consumers reach the two propFSM pointees through it) because every link derives from
// fsmEVENT_HANDLER above; every base in every chain sits at offset 0, so the event-forwarding
// upcasts are unchanged. Method bodies remain the fsm re-source frontier. boundary.

template<class K, class V> struct fsmPAIR_HLD; // fsm name->state pair node — boundary (vector element, pointer-backed store)
struct fsmDESC_HLD;                            // fsm descriptor — boundary (pointer only)

// One FSM state node. DB-verified layout (types_members fsmSTATE, size 20):
// <base> fsmEVENT_HANDLER@0 (12B), id@0xC (u8), eventHandler@0x10 (fsmEVENT_HANDLER *).
struct fsmSTATE : fsmEVENT_HANDLER {
    unsigned char     id;           // 0x0C state id
    unsigned char     pad[3];       // 0x0D alignment padding (explicit in the DB record)
    fsmEVENT_HANDLER *eventHandler; // 0x10 handler events on this state are forwarded to
};

// A state that owns child states. DB-verified layout (types_members fsmSTATE_CONTAINER, size 40):
// <base> fsmSTATE@0 (20B), stateList@0x14 (dsVECTOR<fsmPAIR_HLD<dsSTRID,fsmSTATE>,8>, 20B).
struct fsmSTATE_CONTAINER : fsmSTATE {
    dsVECTOR<fsmPAIR_HLD<dsSTRID, fsmSTATE>, 8> stateList; // 0x14 named child states
};

// The body FSM state machine. DB-verified layout (types_members fsmSTATE_MANAGER, size 196):
// <base> fsmEVENT_HANDLER@0 (12B), stackState@0xC (fsmSTATE_MANAGER::STATE_STACK, 16B),
// stateList@0x1C, stateGoalQueue@0x30, isSyncMode@0x44, isSyncModeOvr@0x45, isClientMode@0x46,
// idxCommonState@0x48, eventHandler@0x4C (fsmCB_HANDLER<dsSTRID>, 92B), idUndef@0xA8,
// isFrameUpdate@0xAC, eventQueue@0xB0 (dsVECTOR<dsSTRID,8>, 20B).
struct fsmSTATE_MANAGER : fsmEVENT_HANDLER {
    // DB nested fsmSTATE_MANAGER::STATE_STACK (size 16): stackGoal@0 (dsCONST_ARRAY<int,3>).
    struct STATE_STACK {
        dsCONST_ARRAY<int, 3> stackGoal; // 0x00 pushed goal-state indices
    };

    STATE_STACK                                 stackState;     // 0x0C
    dsVECTOR<fsmPAIR_HLD<dsSTRID, fsmSTATE>, 8> stateList;      // 0x1C named states
    dsVECTOR<dsSTRID, 8>                        stateGoalQueue; // 0x30 pending goal states
    bool                                        isSyncMode;     // 0x44
    bool                                        isSyncModeOvr;  // 0x45
    bool                                        isClientMode;   // 0x46
    unsigned char                               pad[1];         // 0x47 alignment padding
    int                                         idxCommonState; // 0x48
    fsmCB_HANDLER<dsSTRID>                      eventHandler;   // 0x4C event-callback registry (92B)
    int                                         idUndef;        // 0xA8
    bool                                        isFrameUpdate;  // 0xAC
    unsigned char                               pad2[3];        // 0xAD alignment padding
    dsVECTOR<dsSTRID, 8>                        eventQueue;     // 0xB0 queued events
}; // size 196

// A state that holds (delegates to) a whole nested state machine. DB-verified layout
// (types_members fsmSTATE_FSM_HLD, size 56): <base> fsmSTATE_CONTAINER@0 (40B), fsm@0x28
// (dsSHARED_PTR<fsmSTATE_MANAGER,0,Deleter<fsmSTATE_MANAGER>>, 8B), isSyncBySelf@0x30,
// isForceExit@0x31, isDeactivated@0x32, descStateDerivedFrom@0x34 (const fsmDESC_HLD *).
struct fsmSTATE_FSM_HLD : fsmSTATE_CONTAINER {
    dsSHARED_PTR<fsmSTATE_MANAGER, 0, Deleter<fsmSTATE_MANAGER> > fsm; // 0x28 owned nested FSM
    bool               isSyncBySelf;         // 0x30
    bool               isForceExit;          // 0x31
    bool               isDeactivated;        // 0x32
    unsigned char      pad[1];               // 0x33 alignment padding
    const fsmDESC_HLD *descStateDerivedFrom; // 0x34
}; // size 56
