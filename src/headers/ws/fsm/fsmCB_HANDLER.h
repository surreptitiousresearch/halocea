#pragma once
#include "fsmEVENT_BASE.h"
#include "../ds/dsSTRID.h"
// ws-engine fsm boundary — the event-callback handler template keyed by interned event id
// (fsmCB_HANDLER<KEY>). Only the templated RegisterCB entry point reached by the aiWEAPON
// target/spread subscription is modelled; the full body/layout is the next re-source frontier, so
// this is a BOUNDARY (do not treat FULL_HDL_2_CB / the class body as an authoritative layout).

template<class KEY>
struct fsmCB_HANDLER {
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

// fsm weapon-event id accessors (real statics). boundary — bodies external to this batch.
// DB types_members fsmWEAPON_EVENT: one row — the empty fsmEVENT_BASE base at offset 0.
struct fsmWEAPON_EVENT : fsmEVENT_BASE {
    // ?request_tgt_@fsmWEAPON_EVENT@@SAABVdsSTRID@@XZ
    static const dsSTRID &request_tgt_();
    // ?request_sprd_@fsmWEAPON_EVENT@@SAABVdsSTRID@@XZ
    static const dsSTRID &request_sprd_();
};
