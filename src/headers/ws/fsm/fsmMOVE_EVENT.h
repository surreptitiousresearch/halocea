#pragma once
#include "../ds/dsSTRID.h"
#include "../m3d/m3dV.h"
// ws-engine fsm — the "move" body-FSM event family. Only the id accessors reached by the aiBODY
// locomotion forwarders are modelled; the rest is the next frontier.  boundary.

struct fsmMOVE_EVENT {
    // Pos-goal parameter block for the "set" event (fsmMOVE_EVENT::PARAM): goal position, whether it
    // is the last path point, and the goal facing direction. Matches the buffer the aiBODY
    // pos-goal forwarder fills. ??0PARAM@fsmMOVE_EVENT@@QAA@ABUm3dV@@_N@Z ctor takes (pos, isLast).
    struct PARAM {
        m3dV pos;          // 0x00 goal position
        bool isLastPoint;  // 0x0C last point of the path
        m3dV dir;          // 0x10 goal facing direction
    };

    // ?set_@fsmMOVE_EVENT@@SAABVdsSTRID@@XZ @ 0x829BDEA8 — interned "set" (pos-goal) id.  boundary.
    static const dsSTRID &set_();
    // ?cdt_processed_@fsmMOVE_EVENT@@SAABVdsSTRID@@XZ — interned "cdt_processed" id.  boundary.
    static const dsSTRID &cdt_processed_();

    // Non-static id-getters used as pointer-to-member arguments to fsmEVENT_HANDLER::SendEvent.
    // ?move_up@fsmMOVE_EVENT@@QAAABVdsSTRID@@XZ / ?move_down@... — interned "move_up"/"move_down" ids.
    const dsSTRID &move_up();
    const dsSTRID &move_down();

    // Pos-goal parameter block (fsmMOVE_EVENT::PARAM): m3dV pos, bool isLastPoint, m3dV dir.
    // ??0PARAM@fsmMOVE_EVENT@@QAA@ABUm3dV@@_N@Z ctor takes (pos, isLastPoint).
};
