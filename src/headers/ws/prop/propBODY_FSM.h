#pragma once
// ws-engine prop: the body-specific property FSM (a propFSM specialisation). DB layout derives
// propFSM@0 with bodyHld@0x9C etc.; this batch only needs the Cmd2Body accessor, so the type is
// modeled minimally as a boundary. boundary — full layout is the next frontier.

struct mdlCMD2BODY;

struct propBODY_FSM {
    // 0x829F04A8 (?Cmd2Body@propBODY_FSM@@QAAAAUmdlCMD2BODY@@XZ) — the per-frame command block this
    // FSM feeds to the model. boundary — body external to this batch.
    mdlCMD2BODY &Cmd2Body();
};
