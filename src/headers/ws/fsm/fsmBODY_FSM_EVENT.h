#pragma once
#include "fsmEVENT_BASE.h"
// ws-engine fsm: body-FSM event id accessors (static interned dsSTRID getters). Only the ho-enter
// param query id reached by the aiHO_SYS drain is modeled here; the remaining ids are the next
// frontier. Bodies external (fsm subsystem). boundary.

struct dsSTRID;

// DB types_members fsmBODY_FSM_EVENT: one row — the empty fsmEVENT_BASE base at offset 0.
struct fsmBODY_FSM_EVENT : fsmEVENT_BASE {
    // ?get_ho_enter_param_@fsmBODY_FSM_EVENT@@SAABVdsSTRID@@XZ — interned "get_ho_enter_param" id.
    static const dsSTRID &get_ho_enter_param_(); // boundary
    // ?get_cur_aiming_@fsmBODY_FSM_EVENT@@SAABVdsSTRID@@XZ — interned "get_cur_aiming" id.  boundary.
    static const dsSTRID &get_cur_aiming_();
    // ?disable_hits_@fsmBODY_FSM_EVENT@@SAABVdsSTRID@@XZ — interned "disable_hits" id.  boundary.
    static const dsSTRID &disable_hits_();
};
