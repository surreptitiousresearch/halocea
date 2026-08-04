#pragma once
#include "aiISA_IFACE.h"
#include "AI_ALERT.h"
#include "../ds/dsFLAGS.h"
#include "../ds/dsTSTRING.h"
#include "../m3d/m3dV.h"
// ws-engine ai08 (a8_isa.cpp): aiISA — the concrete interop/social-awareness mind module bound to a
// brain. DB (types_members aiISA, size 0xC): aiISA_IFACE base @0x00, state @0x08.

// AI_ISAST — per-report "this alert kind is enabled" bits plus the ALERTED latch. DB-verified
// (types_enum_values AI_ISAST). Kept here (single small consumer); no separate header exists.
enum AI_ISAST {
    AI_ISAST_REPORT_SEE       = 0x00000001,
    AI_ISAST_REPORT_HEAR      = 0x00000002,
    AI_ISAST_REPORT_DAMAGE    = 0x00000004,
    AI_ISAST_REPORT_PHYS_OBJ  = 0x00000008,
    AI_ISAST_REPORT_INTEROP   = 0x00000100,
    AI_ISAST_REPORT_GRAVLINK  = 0x00100000,
    AI_ISAST_ALERTED          = 0x10000000,
    AI_ISAST_LASTSTATE        = 0x10000001,
};

struct dmgDAMAGE;      // ws/dmg — damage record (NotifyDMG arg)   boundary (const ref)
struct entSND_NOTICE;  // ws/ent — sound-notice event (NotifySND arg; full def entSND_NOTICE.h)

struct aiISA : aiISA_IFACE {
    dsFLAGS<enum AI_ISAST, int> state; // 0x08 — enabled-report bits + alerted latch

    // Parameters passed to aiISA::Alert. DB (types_members aiISA::ALERT_PARAMS, size 0x18).
    struct ALERT_PARAMS {
        const AI_ALERT type;              // 0x00 alert kind
        const m3dV pos;                   // 0x04 world position of the stimulus
        const dsTSTRING<char> reason;     // 0x10 human-readable reason tag
        const bool isAlertTeam;           // 0x14 propagate to the whole team
        const bool isUseDelay;            // 0x15 route through the delayed-alert path
    };

    // 0x832A1974 (a8_isa.cpp:0x9E) — ctor; installs this module on `pBrain`.  boundary.
    aiISA(aiBRAIN *pBrain);

    // 0x8329FF50 (?Alert@aiISA@@MAA_NABUALERT_PARAMS@1@@Z) — protected virtual: raise an alert with
    // the given params. Returns true if the alert was accepted.
    // REVERSED: src/ws/ai/aiISA__Alert.cpp.
    bool Alert(const ALERT_PARAMS &params);
    // vtbl+0x20 (?IsAlerted@aiISA@@...) — true when the unit is currently alerted.  boundary.
    bool IsAlerted();

    // 0x832A0030 (?SetPar@aiISA@@UAA_NABV?$dsTSTRING@D@@M@Z) — virtual: apply a named alert-enable
    // tunable; true if consumed. REVERSED: src/ws/ai/aiISA__SetPar.cpp.
    bool SetPar(const dsTSTRING<char> &key, float val);
    // 0x832A03B8 (?IsEnabledAlert@aiISA@@UBA_NW4AI_ALERT@@@Z) — virtual const: is `alert` currently
    // enabled for reporting. REVERSED: src/ws/ai/aiISA__IsEnabledAlert.cpp.
    bool IsEnabledAlert(AI_ALERT alert) const;
    // 0x832A04E8 (?EnableAlert@aiISA@@UAAXW4AI_ALERT@@_N@Z) — virtual: enable/disable `alert`.
    // REVERSED: src/ws/ai/aiISA__EnableAlert.cpp.
    void EnableAlert(AI_ALERT alert, bool enable);
    // 0x832A0910 (?NotifySND@aiISA@@UAAXABVentSND_NOTICE@@@Z) — virtual: a sound was heard; alert if
    // within the matching hearing radius. REVERSED: src/ws/ai/aiISA__NotifySND.cpp.
    void NotifySND(const entSND_NOTICE &notice);
    // 0x832A0FF0 (?ProcessVision@aiISA@@IAAXXZ) — protected: if the enemy is visible, raise a SEE
    // alert. REVERSED: src/ws/ai/aiISA__ProcessVision.cpp.
    void ProcessVision();

    // --- REVERSED in this batch ---
    // 0x832A11F0 — src/ws/ai/aiISA__NotifyInterop.cpp
    void NotifyInterop(const m3dV &pos);
    // 0x832A12D0 — src/ws/ai/aiISA__NotifyPhysObject.cpp
    void NotifyPhysObject(const m3dV &pos);
    // 0x832A13C8 — src/ws/ai/aiISA__NotifyScriptAlert.cpp
    void NotifyScriptAlert();
    // 0x832A1550 — src/ws/ai/aiISA__NotifyDMG.cpp
    void NotifyDMG(const dmgDAMAGE &dmg);
    // 0x832A14B0 — src/ws/ai/aiISA__DbgAppendLines.cpp
    void DbgAppendLines(dsTSTRING<char> &line);
};
