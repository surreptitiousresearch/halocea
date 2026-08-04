#pragma once
#include "../ds/dsTSTRING.h"
#include "../ds/dsVECTOR.h"
// ws-engine ai08: base class for a brain's weapon-behaviour module.
// DB-verified layout (types_members aiWEAPON_BASE): __vftable@0 — size 4.

struct aiWEAPON_BASE_vtbl;
struct aiSTRIKE_FILTER; // aiSTRIKE_FILTER.h — strike/attack query filter   boundary (ref)
struct wpnS_DESC;       // wpn subsystem — weapon strike descriptor          boundary (opaque)
struct m3dV;            // ws m3d — 3-vector                                 boundary (fwd)
struct propWPN_AI_USABLE; // prop subsystem — AI-usable weapon controller   boundary (fwd)

struct aiWEAPON_BASE {
    aiWEAPON_BASE_vtbl *__vftable; // 0x00

    // 0x83178A58 (?GetString@aiWEAPON_BASE@@UBA?AV?$dsTSTRING@D@@ABV2@@Z) — virtual, const.
    // Base default: ignore `desc` and return the shared empty string (sret).
    virtual dsTSTRING<char> GetString(const dsTSTRING<char> &desc) const;

    // 0x83174110 (?OnDieStart@aiWEAPON_BASE@@UAAXXZ) — virtual, no args: react to the owner's death
    // starting (base default no-op; overridden in aiWEAPON). Used by aiDIE::Start.  boundary.
    void OnDieStart();

    // 0x83174118 (?ProcessFrame@aiWEAPON_BASE@@UAAXM@Z) — virtual: per-frame weapon-module update.
    // Used by aiBODY_IFACE::ProcessFrame.  boundary.
    void ProcessFrame(float dt);

    // True while the weapon module is currently firing. Used by aiWATCHER_BRAIN::IsShooting.
    // boundary (body external to this batch).
    bool IsShootingNow();

    // ?GetSDescVectorByFilter@aiWEAPON_BASE@@UBAXAAUaiSTRIKE_FILTER@@AAV?$dsVECTOR@PBUwpnS_DESC@@$07@@@Z
    // — virtual, const: collect every strike descriptor matching `filter` into `out`. Declared as a
    // plain member (matching the sibling boundary convention). boundary — body external to this batch.
    void GetSDescVectorByFilter(aiSTRIKE_FILTER &filter,
                                dsVECTOR<const wpnS_DESC *, 8> &out) const;

    // ?GetPosDirTip@aiWEAPON_BASE@@UBA_NAAUm3dV@@0@Z — virtual const: write the weapon's muzzle-tip
    // position and firing direction into `pos`/`dir`; returns true when supplied. Used by the ai08
    // debug weapon-render path and aiBRAIN::GetFaceParam. boundary — body external to this batch.
    // DEVIATION: return type corrected void->bool per DB mangle (_N) — callers test the result.
    bool GetPosDirTip(m3dV *pos, m3dV *dir);

    // vtbl slot 0xA8 (?SetLOD@aiWEAPON_BASE@@UAAXH@Z) — set the weapon module's level-of-detail.
    // Used by aiBRAIN::implSetLOD.  boundary — body external to this batch.
    void SetLOD(int idx);

    // True when the weapon module currently has no target/weapon (the debug "absent" state). Used
    // by aiDEBUG::RenderWeaponGroup. boundary — body external to this batch.
    bool IsAbsent();

    // Recompute the debug/trajectory scratch variables for the weapon. Called from the debug
    // trajectory-checker path. boundary — body external to this batch.
    void DbgRecalcVars();

    // ?NoticeEnemyChanged@aiWEAPON_BASE@@UAAXXZ @ 0x83174270 — virtual, void, no args: react to the
    // brain's tracked enemy changing. Called from aiMIND::NoticeEnemyChanged. Declared as a plain
    // member (matching the sibling boundary convention). boundary — body external to this batch.
    void NoticeEnemyChanged();

    // ?UnSubscribeForTargetAndSpread@aiWEAPON_BASE@@UAAXPAVpropWPN_AI_USABLE@@@Z @ 0x83295C88 —
    // virtual: unhook this module from the weapon controller's target/spread request FSM events.
    void UnSubscribeForTargetAndSpread(propWPN_AI_USABLE *ctrl);

    // ?UpdateVarsCache@aiWEAPON_BASE@@UAAXXZ — virtual, no args: refresh the module's cached weapon
    // vars. Used by aiBODY_IFACE::FillCache.  boundary — body external to this batch.
    void UpdateVarsCache();

    // ?SetPar@aiWEAPON_BASE@@UAA_NABV?$dsTSTRING@D@@M@Z — virtual: apply a named float tunable to the
    // weapon module; returns whether it was consumed. Used by aiBODY::SetPar.  boundary.
    bool SetPar(const dsTSTRING<char> &key, float val);

    // ?EnableStrike@aiWEAPON_BASE@@UAA_NABV?$dsTSTRING@D@@_N@Z — virtual: enable (bEnable==true) or
    // disable the named strike; returns false when no such strike exists. Used by aiBRAIN::implEnableStrike
    // / implDisableStrike.  boundary — body external to this batch.
    bool EnableStrike(const dsTSTRING<char> &name, bool bEnable);
    // ?LockStrike@aiWEAPON_BASE@@UAA_NABV?$dsTSTRING@D@@M@Z — virtual: lock the named strike for `time`
    // seconds; returns false when no such strike exists. Used by aiBRAIN::implLockStrike. boundary.
    bool LockStrike(const dsTSTRING<char> &name, float time);

    // vtbl+0xB4 (?DbgLeaveOneEnabled@aiWEAPON_BASE@@UAAXABV?$dsTSTRING@D@@@Z) — debug: leave only the
    // strike named `name` enabled. Declared as a plain member (boundary convention). Used by
    // aiBRAIN::implDbgStrike.  boundary — body external to this batch.
    void DbgLeaveOneEnabled(const dsTSTRING<char> &name);
};
