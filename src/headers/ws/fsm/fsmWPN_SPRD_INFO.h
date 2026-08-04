#pragma once
// ws-engine fsm: callback param block a weapon FSM raises to request the AI's current spread scale.
// DB-verified layout (types_members fsmWPN_SPRD_INFO): fsmCB_PARAM_BASE base@0 (empty),
// spreadScale@0 — size 4.

struct fsmWPN_SPRD_INFO {
    float spreadScale; // 0x00 requested spread multiplier
};
