#pragma once
#include "fsmEVENT_BASE.h"
#include "../ds/dsSTRID.h"
#include "../m3d/m3dV.h"
// ws-engine fsm — the "move" body-FSM event family. Only the id accessors reached by the aiBODY
// locomotion forwarders are modelled; the rest is the next frontier.  boundary.

// DB types_members fsmMOVE_EVENT: one row — the empty fsmEVENT_BASE base at offset 0.
struct fsmMOVE_EVENT : fsmEVENT_BASE {
    // Pos-goal parameter block for the "set" event.
    // DB-verified layout (types_members fsmMOVE_EVENT::PARAM, types size 44): posGoal@0 (m3dV),
    // isLastPoint@0xC (bool), dirLocalCS@0x10 (m3dV), dirCamAim@0x1C (m3dV), rotSpeed@0x28 (float).
    // ??0PARAM@fsmMOVE_EVENT@@QAA@ABUm3dV@@_N@Z ctor takes (posGoal, isLastPoint).
    // DEVIATION: the first two vector members keep the call-site names `pos`/`dir` rather than the
    // DB's `posGoal`/`dirLocalCS` — the only writer is src/ws/ai/aiBODY__SendFsmEventPosGoal.cpp and
    // src/ws/ai is under a standing FULL STOP, so the rename is deferred with the DB names recorded
    // here. The last two members were absent, which under-sized the caller's stack block by 16 bytes
    // against the 44 the FSM reads.
    struct PARAM {
        m3dV  pos;          // 0x00 goal position          (DB: posGoal)
        bool  isLastPoint;  // 0x0C last point of the path
        m3dV  dir;          // 0x10 goal facing direction  (DB: dirLocalCS)
        m3dV  dirCamAim;    // 0x1C camera aim direction
        float rotSpeed;     // 0x28 turn rate toward the goal facing
    };

    // ?set_@fsmMOVE_EVENT@@SAABVdsSTRID@@XZ @ 0x829BDEA8 — interned "set" (pos-goal) id.  boundary.
    static const dsSTRID &set_();
    // ?cdt_processed_@fsmMOVE_EVENT@@SAABVdsSTRID@@XZ — interned "cdt_processed" id.  boundary.
    static const dsSTRID &cdt_processed_();

    // Non-static id-getters used as pointer-to-member arguments to fsmEVENT_HANDLER::SendEvent.
    // ?move_up@fsmMOVE_EVENT@@QAAABVdsSTRID@@XZ / ?move_down@... — interned "move_up"/"move_down" ids.
    const dsSTRID &move_up();
    const dsSTRID &move_down();
};
