#pragma once
// ws-engine ai08: reload-timing helper owned by a weapon module. DB-verified layout
// (types_members aiRELOADER): pBrain@0, pWpn@4 — size 8.

struct aiBRAIN;           // ai08 — owning brain (fwd)
struct aiWEAPON;          // ai08 — owning weapon module (fwd)
struct propWPN_AI_USABLE; // prop — AI-usable weapon controller (fwd)

struct aiRELOADER {
    aiBRAIN  *pBrain; // 0x00 owning brain
    aiWEAPON *pWpn;   // 0x04 owning weapon module

    // 0x832B3620 (?IsLoaded@aiRELOADER@@IBA_NPBVpropWPN_AI_USABLE@@H@Z) — const: true when the
    // controller's ammo clip is loaded above `perc_ammo_left` percent (or is clip-less / absent).
    // REVERSED: aiRELOADER__IsLoaded.cpp.
    bool IsLoaded(const propWPN_AI_USABLE *ctrl, int perc_ammo_left) const;

    // 0x832B36F8 (?IsNeedToReload@aiRELOADER@@QBA_NPBVpropWPN_AI_USABLE@@H@Z) — const: true when the
    // controller exists and its clip is at/below `perc_ammo_left` percent. REVERSED: aiRELOADER__IsNeedToReload.cpp.
    bool IsNeedToReload(const propWPN_AI_USABLE *ctrl, int perc_ammo_left) const;

    // 0x832B37B0 (?ConsiderReload@aiRELOADER@@QAAXPAVpropWPN_AI_USABLE@@H@Z) — when the controller's
    // clip is at/below `perc_ammo_left`, run a full reload (descriptor flag set) or an instant reload.
    // REVERSED: aiRELOADER__ConsiderReload.cpp.
    void ConsiderReload(propWPN_AI_USABLE *ctrl, int perc_ammo_left);
};
