#pragma once
#include "../ds/dsSTRID.h"
// ws-engine fsm — the base body-FSM event family. Only the "reset" id accessor reached by
// aiBODY::Transport is modelled here.  boundary.

struct fsmFSM_EVENT {
    // ?reset_@fsmFSM_EVENT@@SAABVdsSTRID@@XZ — interned "reset" event id.  boundary.
    static const dsSTRID &reset_();
};
