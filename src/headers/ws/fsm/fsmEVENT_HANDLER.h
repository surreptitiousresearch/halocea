#pragma once
#include "../ds/dsSTRID.h"
// ws-engine fsm — the event handler owned by a body FSM state object (fsmSTATE_MANAGER /
// fsmSTATE_FSM_HLD both derive it). Only the entry points the aiBODY event-forwarding methods reach
// are modelled; the full layout/vtable is the next frontier.  boundary.

struct fsmEVENT_PARAM_BASE; // fsm/fsmAIMING_EVENT.h — empty base of every FSM event param block

// ?...@fsmEVENT_HANDLER@@... — the concrete handler. __vftable@0. Virtual slots reached by aiBODY
// are dispatched by their DB-verified index (like aiBODY_IFACE), without fabricating slot names.
struct fsmEVENT_HANDLER {
    void *__vftable; // 0x00

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

// The two body-FSM state objects a propFSM points at; each is-a fsmEVENT_HANDLER. Full layouts are
// the next frontier — modelled as the base only so the event-forwarding upcast is exact. boundary.
struct fsmSTATE_MANAGER : fsmEVENT_HANDLER {};
struct fsmSTATE_FSM_HLD : fsmEVENT_HANDLER {};
