#pragma once
#include "../ds/dsSTRID.h"
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

// The two body-FSM state objects a propFSM points at; each is-a fsmEVENT_HANDLER. Both are modelled
// as the fsmEVENT_HANDLER sub-object only, which is what the event-forwarding upcast needs and is
// byte-exact for it — every base in both chains sits at offset 0. Their trailing state is the fsm
// re-source frontier, so it is carried here as DB evidence rather than as members. boundary.
//
// DB types_members fsmSTATE_MANAGER (types size 196): <base> fsmEVENT_HANDLER@0 (12B) —
// the base the header spells — then stackState@0xC (fsmSTATE_MANAGER::STATE_STACK, 16B),
// stateList@0x1C (dsVECTOR<fsmPAIR_HLD<dsSTRID,fsmSTATE>,8>), stateGoalQueue@0x30
// (dsVECTOR<dsSTRID,8>), isSyncMode@0x44, isSyncModeOvr@0x45, isClientMode@0x46,
// idxCommonState@0x48, eventHandler@0x4C (fsmCB_HANDLER<dsSTRID>, 92B), idUndef@0xA8,
// isFrameUpdate@0xAC, eventQueue@0xB0 (dsVECTOR<dsSTRID,8>).
struct fsmSTATE_MANAGER : fsmEVENT_HANDLER {};

// DB types_members fsmSTATE_FSM_HLD (types size 56) names fsmSTATE_CONTAINER as the direct base;
// the header flattens the chain, which is exact because every link starts at offset 0:
// fsmSTATE_FSM_HLD(56) : fsmSTATE_CONTAINER(40) : fsmSTATE(20) : fsmEVENT_HANDLER(12), with
// fsmSTATE = {<base> fsmEVENT_HANDLER@0, id@0xC (u8), eventHandler@0x10 (fsmEVENT_HANDLER *)} and
// fsmSTATE_CONTAINER = {<base> fsmSTATE@0, stateList@0x14 (dsVECTOR<fsmPAIR_HLD<dsSTRID,fsmSTATE>,8>)}.
// fsmSTATE_FSM_HLD's own tail: fsm@0x28 (dsSHARED_PTR<fsmSTATE_MANAGER,...>), isSyncBySelf@0x30,
// isForceExit@0x31, isDeactivated@0x32, descStateDerivedFrom@0x34 (const fsmDESC_HLD *).
struct fsmSTATE_FSM_HLD : fsmEVENT_HANDLER {};
