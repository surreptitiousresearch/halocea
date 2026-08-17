#pragma once
#include "fsmEVENT_BASE.h"
#include "../ds/dsSTRID.h"
#include "../ds/MAP.h"
// ws-engine fsm boundary — the event-callback handler template keyed by interned event id
// (fsmCB_HANDLER<KEY>). The templated RegisterCB entry point reached by the aiWEAPON
// target/spread subscription is modelled, plus the DB-verified data layout (types_members
// fsmCB_HANDLER<dsSTRID>, size 92): __vftable@0, cbMap@4 (ds::MAP<dsSTRID,fsmMEM_CB_BASE*,
// ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>, 84B), idLast@0x58. Method bodies stay boundary.
// Split out of fsmCB_HANDLER.h (2026-08-17) so fsmEVENT_HANDLER.h can reach the template
// WITHOUT dragging `struct fsmWEAPON_EVENT` into the ws/ai include chain, where it collides
// (C2757) with the FULL-STOP boundary header's `namespace fsmWEAPON_EVENT` spelling of the
// same DB statics.

struct fsmMEM_CB_BASE; // fsm member-callback node base — boundary (pointer-only map value)

template<class KEY>
struct fsmCB_HANDLER {
    struct VTBL; // per-instantiation dispatch table (DB: fsmCB_HANDLER<dsSTRID>_vtbl)
    VTBL *__vftable;                                                            // 0x00
    ds::MAP<KEY, fsmMEM_CB_BASE *, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> cbMap; // 0x04 (84B)
    unsigned int idLast;                                                        // 0x58 last-issued registration id
    // Registration cookie pieces returned by RegisterCB (by value; the sret temporary is discarded
    // at the call site). DB-verified layouts (types_members fsmCB_HANDLER<dsSTRID>::HDL_2_CB size 8;
    // ::FULL_HDL_2_CB size 12 = HDL_2_CB base + handler@8).
    struct HDL_2_CB {
        unsigned int id;      // 0x00 registration id (handler's idLast at registration)
        dsSTRID      cbEvent; // 0x04 the subscribed event
    };
    struct FULL_HDL_2_CB : HDL_2_CB {
        fsmCB_HANDLER<KEY> *handler; // 0x08 back-pointer to the issuing handler
    };

    // Register `func` (a member of host `T`, taking a PARAM*) as the callback for `event`, hosted by
    // `obj`. Returns the registration handle. boundary — body external to this batch.
    template<class T, class PARAM>
    FULL_HDL_2_CB RegisterCB(const dsSTRID &event, T *obj, void (T::*func)(PARAM *));
};
