#pragma once

// ws-engine ned (nav editor) subsystem — boundary declaration of nedNAV_EDITOR. The planner holds
// one by pointer (aiPLANNER::navEditor) and drives it through the small virtual surface below. The
// editor's real layout and method bodies are the next frontier; only the interface the ai08
// dispatch methods (a8_plan_msg.cpp) call is declared here. Always used by pointer, so no fields.

struct gsGEOM_ANCHOR;
struct m3dV;

struct nedNAV_EDITOR {
    // 0x83213AB0 (?IsEnabled@nedNAV_EDITOR@@UBA_NXZ) — true while the editor is active.
    bool IsEnabled() const;
    // 0x8320E4E8 (?NoticePlrCross@nedNAV_EDITOR@@UAAXABVgsGEOM_ANCHOR@@ABUm3dV@@@Z) — the player's
    // debug cursor crossed `anchor` at world point `pt`.
    void NoticePlrCross(const gsGEOM_ANCHOR &anchor, const m3dV &pt);
    // 0x8320E6C0 (?Visualize@nedNAV_EDITOR@@UAAXXZ) — draw the editor's debug overlay.
    void Visualize();
    // 0x8320E4C8 (?ProcessMsg@nedNAV_EDITOR@@UAAXH@Z) — handle engine message `msg`.
    void ProcessMsg(int msg);
    // 0x8320E550 (?InitLevel@nedNAV_EDITOR@@UAAXXZ) — per-level init.
    void InitLevel();
    // 0x8320F560 (?TermLevel@nedNAV_EDITOR@@UAAXXZ) — per-level teardown.
    void TermLevel();
};
