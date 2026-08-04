#pragma once
#include "aiAPPROACH_BASE.h"
#include "AI_APST.h"
#include "../ds/dsFLAGS.h"
#include "../ds/dsTSTRING.h"
// ws-engine ai08: the concrete "approach" behaviour module. When enemies/players/allies enter (or
// leave) a radius around the owning unit it fires the descriptor OnApproach event on the brain.
// DB-verified layout (types_members aiAPPROACH / aiAPPROACH_IFACE), size 0x14:
//   aiAPPROACH_IFACE base@0 (aiAPPROACH_BASE@0 + pBrain@4), state@8, name@0xC, radius@0x10.

struct aiBRAIN;
struct entENTITY;
struct psSECTION;

// aiAPPROACH_IFACE — binds an approach-base module to its owning brain. DB size 8.
struct aiAPPROACH_IFACE : aiAPPROACH_BASE {
    aiBRAIN *pBrain; // 0x04 owning brain
};

struct aiAPPROACH : aiAPPROACH_IFACE {
    dsFLAGS<AI_APST, int> state;  // 0x08 approach state bitmask
    dsTSTRING<char>       name;   // 0x0C name of the current "approach" section
    float                 radius; // 0x10 trigger radius

    // 0x8329A758 (??0aiAPPROACH@@QAA@PAVaiBRAIN@@@Z) — construct bound to owning brain.
    aiAPPROACH(aiBRAIN *);

    // 0x8329A700 (?CheckDistance@aiAPPROACH@@IBA_NPBVentENTITY@@@Z) — protected const: true when
    // `entity` has an instance and lies within `radius` of the brain. REVERSED.
    bool CheckDistance(const entENTITY *entity) const;
    // 0x8329A8D8 (?CheckDistance@aiAPPROACH@@IAAXXZ) — protected: re-evaluate the SOMEONE_IN_ZONE
    // flag against every watched player/ally. REVERSED.
    void CheckDistance();
    // 0x8329AB38 (?ProcessInZone@aiAPPROACH@@IAAXXZ) — protected. REVERSED.
    void ProcessInZone();
    // 0x8329AB88 (?ProcessOutZone@aiAPPROACH@@IAAXXZ) — protected. REVERSED.
    void ProcessOutZone();
    // 0x8329AD60 (?ParsePS@aiAPPROACH@@IAA_NABVpsSECTION@@@Z) — protected: parse radius / notice /
    // isActOnce from a config section. Returns non-zero on parse error. REVERSED.
    bool ParsePS(const psSECTION &ps);
    // 0x8329B098 (?ProcessFrame@aiAPPROACH@@UAAXXZ) — vtbl 0x04: per-frame zone transition. REVERSED.
    void ProcessFrame();
    // 0x8329B120 (?SetByName@aiAPPROACH@@UAA_NABV?$dsTSTRING@D@@@Z) — vtbl 0x08: load the named
    // "approach" section from the brain descriptor and apply it. REVERSED.
    bool SetByName(const dsTSTRING<char> &name);
};
