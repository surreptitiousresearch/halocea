#pragma once
#include "aiWEAPON_BASE.h"
#include "../ds/dsCONST_ARRAY.h"
#include "../ds/dsVECTOR.h"
#include "../ds/WEAK_PTR.h"
#include "aiS_VARS.h"
#include "aiS_TIE.h"
#include "aiTRAJ_CHECKER.h"
#include "aiRELOADER.h"
#include "aiPHASER.h"
#include "aiSTRIKE_FILTER.h"
#include "aiW_IDX.h"
#include "../ds/dsVECTOR.h"
#include "../wpn/aimCHECK_TYPE.h"
#include "aiSTRIKE.h"
#include "../m3d/m3dV.h"
// ws-engine ai08: the concrete "full" weapon module a brain's body drives (derives from
// aiWEAPON_BASE). Constructed with its owning aiBRAIN* by aiBRAIN::SetupBody.
// DB-verified layout (types_members aiWEAPON): size 4996. ctrlsList is kept byte-exact opaque
// (its element type aiWEAPON::CTRL_INFO is the next frontier and is not exercised by this batch).

struct aiBRAIN;              // ai08 — owning brain (fwd)
struct wpnLIST_BASE;         // wpn — weapon inventory (fwd)
struct entENTITY;            // ent — entity (fwd, weak-ptr target)
struct propWPN_AI_USABLE;    // prop — AI-usable weapon controller (fwd)
struct wpnWEAPON;            // wpn — a single equipped weapon (fwd)
struct wpnS_DESC;            // wpn — weapon strike descriptor (fwd)
struct atkDSP_INFO;          // wpn/atk — attack-dispatch info (fwd)
struct aiSTRIKE;             // ai08 — resolved strike candidate (fwd)
struct fsmWPN_TGT_INFO;      // fsm — weapon-target request param (fwd)
struct fsmWPN_SPRD_INFO;     // fsm — weapon-spread request param (fwd)
struct fsmWPN_LIST_PARAM_WEAPON; // fsm — weapon add/activate/delete notification param (fwd)
struct m3dV;                 // ws m3d — 3-vector (fwd)

struct aiWEAPON : aiWEAPON_BASE {
    // aiWEAPON::aiS_PHASES — strike phase state (DB types_enum_values).
    enum aiS_PHASES {
        AI_SPH_AIMING       = 0,
        AI_SPH_PERFORMING   = 1,
        AI_SPH_COOLING_DOWN = 2,
        AI_SPH_NONE         = 3,
    };

    // aiWEAPON::CTRL_INFO — one registered weapon controller slot. DB-verified layout
    // (types_members aiWEAPON::CTRL_INFO): ctrl@0, isActive@4, isEnabledByCustomWeapon@5,
    // mayOptByTip@6 — size 8.
    struct CTRL_INFO {
        ds::WEAK_PTR<propWPN_AI_USABLE> ctrl;                   // 0x00 controller weak-ref
        bool                            isActive;               // 0x04
        bool                            isEnabledByCustomWeapon;// 0x05
        bool                            mayOptByTip;            // 0x06 tip cache may use fast path
    };

    aiBRAIN                       *pBrain;            // 0x0004 owning brain
    float                          coeffExagPredict; // 0x0008
    bool                           isOKBlindShoot;    // 0x000C
    bool                           mayShootByScript;  // 0x000D
    dsCONST_ARRAY<aiS_VARS,50>     svarsList;         // 0x0010 per-strike vars pool
    dsVECTOR<CTRL_INFO,8>          ctrlsList;         // 0x120C registered weapon controllers
    wpnLIST_BASE                  *wpnList;           // 0x1220 weapon inventory
    ds::WEAK_PTR<entENTITY>        customWeapon;      // 0x1224
    float                          spreadScale;       // 0x1228
    aiS_TIE                        tieSelected;       // 0x122C currently selected strike tie
    aiTRAJ_CHECKER                 trc;               // 0x1240 trajectory checker
    aiRELOADER                     rld;               // 0x1314 reload helper
    aiPHASER<aiS_PHASES>           strikePhases;      // 0x131C strike phase driver
    float                          timerHackKillHang; // 0x1328
    float                          timerDelayedOSS;   // 0x132C
    aiSTRIKE_FILTER                filterLastUsed;    // 0x1330 last strike filter used

    // ---- ctor + debug (kept for existing opaque-view consumers) ----
    aiWEAPON(aiBRAIN *pBrain);
    void DbgRecalcVars();

    // ---- virtual overrides exercised by this batch ----
    // ?ProcessFrame@aiWEAPON@@UAAXM@Z — per-frame weapon-module update (timed; skipped for a dead
    // brain): refresh vars/trajectory/current-strike, age timers, then resolve the target.
    virtual void  ProcessFrame(float dt);                                   // 0x83296668
    virtual bool  IsShootingNow() const;                                    // 0x83293348
    virtual float CalcHeightAddition(const m3dV &trg) const;                // 0x83293A90
    virtual float CalcHeightAddition(const m3dV &trg, const aiSTRIKE &strike) const; // 0x83293B78
    virtual void  UpdateTimers(float dt);                                   // 0x832939A0
    virtual bool  GetPosDirTip(m3dV &posTip, m3dV &dirTip) const;           // 2-arg (virtual) 0x83295B50
    // ?IsShootingMelee@aiWEAPON@@UBA_NXZ — virtual, const: true while the selected strike is a
    // melee strike (its filter matches wpnS_DESC::MELEE) and the module is currently firing.
    virtual bool  IsShootingMelee() const;                                  // 0x83295CE8
    // ?UpdateVarsCache@aiWEAPON@@UAAXXZ — virtual: refresh per-controller tip caches, then re-resolve
    // the interface/controller weak pointers for every live strike-vars entry.
    virtual void  UpdateVarsCache();                                        // 0x83295DA8
    // ?IsAimed@aiWEAPON@@UBAHAAUm3dV@@@Z — virtual, const: is the weapon currently aimed at world
    // point `trg` (across the selected tie, or any active controller when none selected).
    virtual int   IsAimed(m3dV &trg) const;                                 // 0x83296330

    // ---- strike-tip / aim helpers reversed in this batch (protected in the binary) ----
    bool  GetPosDirTip(const aiS_TIE &tie, m3dV &posTip, m3dV &dirTip) const;// 0x832957E0
    void  UpdateTipCacheFast(const aiW_IDX<propWPN_AI_USABLE> &idxCtrl);     // 0x83295868
    void  UpdateTipCacheSlow(const aiW_IDX<propWPN_AI_USABLE> &idxCtrl);     // 0x832959B8
    bool  IsAimed(const m3dV &posTarget, const aiSTRIKE &strike) const;      // 0x83295A78
    bool  IsAimed(const m3dV &posTarget, const aiS_TIE &tie) const;          // 0x83295FF8
    bool  IsAimed(const m3dV &posTarget, const propWPN_AI_USABLE *ctrl) const;// 0x83296078
    bool  IsNeededTargetAlongWpn() const;                                   // 0x83296140
    void  RecalcTarget();                                                    // 0x83296220

    // ---- internal (protected in the binary) helpers reversed in this batch ----
    aiW_IDX<propWPN_AI_USABLE> GetCtrlIDX(const propWPN_AI_USABLE *ctrl) const; // 0x83292B68
    void  UpdateStrikePhaseCoolingDown();                                   // 0x83292C08
    bool  GetPosDirTip(const propWPN_AI_USABLE *ctrl, m3dV &posTip, m3dV &dirTip) const; // 0x83292CD8
    void  OnTgtRequested(fsmWPN_TGT_INFO *par);                             // 0x83292D40
    void  OnSpreadRequested(fsmWPN_SPRD_INFO *par);                         // 0x83292DA0
    float CalcHeightAddition(const m3dV &trg, const atkDSP_INFO &dspInfo,
                             const wpnS_DESC *desc) const;                  // 0x83293108
    void  UpdateFlagFreezeAim(bool enable);                                 // 0x832932E8
    void  RecalcVars(aiSTRIKE &strike) const;                              // 0x83293398
    bool  GetPosDirTipFromWpn(aiW_IDX<propWPN_AI_USABLE> widx,
                              m3dV &posTip, m3dV &dirTip) const;            // 0x83293C58
    void  RecalcVars(aiS_TIE &tie);                                        // 0x83293D58

    // The vtable slot the decompiler names "GetPosDirTip_2": resolve `widx`, then delegate to the
    // wpn-shooter tip query. boundary — body external to this batch.
    bool  GetPosDirTip(const aiW_IDX<propWPN_AI_USABLE> &widx, m3dV &posTip, m3dV &dirTip) const;

    // ---- per-frame helpers reached by ProcessFrame (bodies external to this batch) — boundaries ----
    void  UpdateVars();
    void  UpdateTraj();
    void  UpdateCurStrike();
    void  SetupTarget();

    // ?SubscribeForTargetAndSpread@aiWEAPON@@UAA_NPAVpropWPN_AI_USABLE@@@Z — virtual: hook the weapon
    // controller's target/spread request FSM events to this module's responders. 0x832967F0.
    virtual bool SubscribeForTargetAndSpread(propWPN_AI_USABLE *ctrl);

    // ---- callee boundaries (bodies external to this batch) ----
    aiW_IDX<propWPN_AI_USABLE> GetCtrlIDX(const wpnWEAPON *wpn) const;
    propWPN_AI_USABLE *GetCtrl(const aiS_TIE &tie) const;
    wpnWEAPON *GetIFace(const aiW_IDX<propWPN_AI_USABLE> &widx) const;
    wpnWEAPON *GetIFace(const aiS_TIE &tie) const;                          // 0x832CAEE0
    aimCHECK_TYPE GetAimCheck(const aiW_IDX<propWPN_AI_USABLE> &idxCtrl) const; // 0x83294FE0
    bool  IsAimedByPosAndDir(const m3dV &posTarget, const aiSTRIKE &strike) const; // 0x83295660
    bool  IsCtrlActive(const wpnWEAPON *wpn) const;                        // 0x83294C78
    bool  GetPosDirTip(const aiSTRIKE &strike, m3dV &posTip, m3dV &dirTip) const; // 0x83295468
    bool  GetAtkInfo(const aiS_TIE *tie, atkDSP_INFO *info) const;
    void  CancelCurStrike(bool skipBodyChecks);
    void  ForgetSelectedStrike();

    // ---- controller/notification + tip methods reversed in this batch (ledger 1014-1027) ----
    // Muzzle tip via the body coordinate system + the controller descriptor's local tip offset.
    bool  GetPosDirTipFromBody(aiW_IDX<propWPN_AI_USABLE> widx,
                               m3dV &posTip, m3dV &dirTip) const;             // 0x83294AE8
    bool  IsCtrlActive(const propWPN_AI_USABLE *ctrl) const;                 // 0x83295058
    void  ActivateCtrl(const propWPN_AI_USABLE *ctrl, bool isActivate);      // 0x83294D50
    propWPN_AI_USABLE *FetchCtrl(fsmWPN_LIST_PARAM_WEAPON *par);             // 0x83294DD8
    void  OnWpnDeleted(fsmWPN_LIST_PARAM_WEAPON *par);                       // 0x83294EF0
    void  OnWpnActivated(fsmWPN_LIST_PARAM_WEAPON *par);                     // 0x83294F58
    void  OnWpnDeactivated(fsmWPN_LIST_PARAM_WEAPON *par);                   // 0x83295240
    void  UpdateStrikePhasePerforming();                                     // 0x832950B0
    // Dispatch a tip query by aim-check type: 1/2 -> body-relative, else -> weapon-shooter.
    bool  GetPosDirTip(aiW_IDX<propWPN_AI_USABLE> widx, aimCHECK_TYPE type,
                       m3dV &posTip, m3dV &dirTip) const;                     // 0x832951A8

    // ---- additional callee boundaries reached by this batch (bodies external) ----
    // ?GetIFace@aiWEAPON@@IBAPBVwpnWEAPON@@V?$WEAK_PTR@VpropWPN_AI_USABLE@@@ds@@@Z @0x832944F0 —
    // resolve a controller weak-ref (consumed by value) to its equipped weapon interface.
    const wpnWEAPON *GetIFace(ds::WEAK_PTR<propWPN_AI_USABLE> ctrl) const;
    void  RemoveCtrlAndStrikes(const aiW_IDX<propWPN_AI_USABLE> &widx);
    void  ReeneableStrikes();
    void  AddCtrlAndStrikes(propWPN_AI_USABLE *ctrl);
    void  UpdateOptimizations();
    bool  IsAbsent(const propWPN_AI_USABLE *ctrl) const;
    using aiWEAPON_BASE::IsAbsent; // keep the inherited 0-arg IsAbsent() visible (name-hiding)
    void  StartStrikeCoolingDown();
    bool  IsNeedToCancelStrikeByAnim() const;
    void  UpdateStrikePhaseAiming();

    // ---- strike-vars / laser / enemy batch reversed this wave (ledger aiWEAPON 0x83293DB8-0x83294A80) ----
    // ?DbgClearCutOffReasons@aiWEAPON@@IAAXXZ @0x83293E58 — clear every strike-vars debug cut-off reason.
    void  DbgClearCutOffReasons();
    // ?StartStrikePerforming@aiWEAPON@@IAA_NXZ @0x83294138 — begin performing the selected strike;
    // registers a strike performance-load sample with the planner. Returns whether a controller existed.
    bool  StartStrikePerforming();
    // ?IsVisionApprovesChooseStrike@aiWEAPON@@IBA_NXZ @0x83294268 — may the current vision state permit
    // choosing a strike now (blind-shoot / enemy-visible / peeker heuristics).
    bool  IsVisionApprovesChooseStrike() const;
    // ?MayChangeEnemy@aiWEAPON@@UBA_NXZ @0x832942D0 — virtual, const: may the brain switch enemies now.
    virtual bool  MayChangeEnemy() const;
    // ?NoticeEnemyChanged@aiWEAPON@@UAAXXZ @0x83294328 — virtual: the tracked enemy changed; cancel a
    // mid-aim strike unless the descriptor allows changing enemy during a strike.
    virtual void  NoticeEnemyChanged();
    // ?ClearVarsCache@aiWEAPON@@UAAXXZ @0x83294570 — virtual: drop every strike-vars cached iface/ctrl
    // weak-ref and reset the tip cache.
    virtual void  ClearVarsCache();
    // ?SwitchLaser@aiWEAPON@@IAAX_N@Z @0x83294668 — start/stop the selected weapon's "pointer" laser sfx.
    void  SwitchLaser(bool enable);
    // ?UpdateLaserState@aiWEAPON@@IAAX_N@Z @0x83294740 — drive the laser on/off from enemy live state.
    void  UpdateLaserState(bool enable);
    // ?GetCtrl@aiWEAPON@@IBAPAVpropWPN_AI_USABLE@@ABV?$aiW_IDX@VpropWPN_AI_USABLE@@@@@Z @0x83294920 —
    // resolve a controller index to its live controller (null when out of range / dead).
    propWPN_AI_USABLE *GetCtrl(const aiW_IDX<propWPN_AI_USABLE> &idx) const;
    // ?IsNeedToCancelAimByAnim@aiWEAPON@@IBA_NXZ — true when the current animation forces an aim cancel.
    // boundary — body external to this batch.
    bool  IsNeedToCancelAimByAnim() const;

    // 0x832B2D40 (?ReloadInstantly@aiWEAPON@@UAAXPAVpropWPN_AI_USABLE@@@Z) — virtual: instantly reload
    // every attack of the controller's equipped weapon (when its shooter is non-empty) by sending a
    // per-attack cmdWPN_RELOAD. REVERSED: src/ws/ai/aiWEAPON__ReloadInstantly.cpp.
    virtual void ReloadInstantly(propWPN_AI_USABLE *ctrl);

    // ---- reload batch reversed this wave (aiRELOADER-driven) ----
    // ?Reload@aiWEAPON@@IAAXPAVpropWPN_AI_USABLE@@@Z @0x832B3268 — full reload: consume a clip on the
    // controller, reload the weapon instantly, then notify the brain (aiBRAIN::OnReload).
    // REVERSED: aiWEAPON__Reload.cpp.
    void  Reload(propWPN_AI_USABLE *ctrl);
    // ?ConsiderReload@aiWEAPON@@IAAXPAVpropWPN_AI_USABLE@@H@Z @0x832B3740 — reload `ctrl` instantly
    // when its clip is at/below `least_percent`. REVERSED: aiWEAPON__ConsiderReload.cpp.
    void  ConsiderReload(propWPN_AI_USABLE *ctrl, int least_percent);
    // ?ConsiderReloads@aiWEAPON@@IAAXH@Z @0x832B3850 — run the instant-reload test over every
    // registered live controller. REVERSED: aiWEAPON__ConsiderReloads.cpp.
    void  ConsiderReloads(int least_percent);
};
