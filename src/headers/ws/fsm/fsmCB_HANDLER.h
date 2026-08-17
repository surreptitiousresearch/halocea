#pragma once
#include "fsmCB_HANDLER_T.h"
// The fsmCB_HANDLER<KEY> template now lives in fsmCB_HANDLER_T.h (see the note there);
// this header keeps its historic surface — template + the weapon-event id statics — for
// its direct includers. Do NOT include this file from any header the ws/ai chain reaches
// transitively: the struct below collides (C2757) with the FULL-STOP boundary header's
// `namespace fsmWEAPON_EVENT` spelling of the same DB statics.

// fsm weapon-event id accessors (real statics). boundary — bodies external to this batch.
// DB types_members fsmWEAPON_EVENT: one row — the empty fsmEVENT_BASE base at offset 0.
struct fsmWEAPON_EVENT : fsmEVENT_BASE {
    // ?request_tgt_@fsmWEAPON_EVENT@@SAABVdsSTRID@@XZ
    static const dsSTRID &request_tgt_();
    // ?request_sprd_@fsmWEAPON_EVENT@@SAABVdsSTRID@@XZ
    static const dsSTRID &request_sprd_();
};
