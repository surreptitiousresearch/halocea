#pragma once
#include "fsmEVENT_BASE.h"
#include "../ds/dsSTRID.h"
// ws-engine fsm — the base body-FSM event family. Only the "reset" id accessor reached by
// aiBODY::Transport is modelled here.  boundary.

// DB types_members fsmFSM_EVENT: one row — the empty fsmEVENT_BASE base at offset 0.
struct fsmFSM_EVENT : fsmEVENT_BASE {
    // ?reset_@fsmFSM_EVENT@@SAABVdsSTRID@@XZ — interned "reset" event id.  boundary.
    static const dsSTRID &reset_();
};
