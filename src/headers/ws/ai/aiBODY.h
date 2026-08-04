#pragma once
#include "aiBODY_IFACE.h"          // base class (also brings ctrlPROP_LIST, dsSHARED_PTR, dsPAIR, dsSTRID)
#include "../ds/dsSHARED_PTR.h"
#include "../ds/Deleter.h"
#include "../ds/dsPAIR.h"
#include "../ds/dsSTRID.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsVECTOR.h"
// ws-engine ai08: the concrete body an aiBRAIN drives — an aiBODY_IFACE plus its FSM/model plumbing
// and uncontrolled-fall bookkeeping. DB-verified layout (types_members aiBODY) — size 0x38:
//   aiBODY_IFACE base@0x00, bodyFsm@0x20, useProcessor@0x24, timeUncontrolled@0x28,
//   timeFalling@0x2C, propListCache@0x30, infoBodyMdl@0x34.

struct propFSM;          // prop — per-body FSM state (fwd, boundary; ptr)
struct usePROCESSOR;     // ai08 — smart-object "use" processor (fwd, boundary; ptr)
struct mdlINFO_BASE;     // mdl/mdlINFO_BASE.h — body model info (ptr member + GetSphereRefine)
struct hoOBJECT;         // ho — hideout object (fwd, boundary; ptr)
struct aiWATCHER;        // ai08 — target watcher (fwd; ptr)
struct smtSMART_OBJ_BASE;// smt — smart object (fwd, boundary; dsSHARED_PTR referent)
struct mdlITRC_HO;       // mdl/mdlITRC_HO.h — hideout ITRC (fwd; dsSHARED_PTR referent)
struct m3dV;             // ws m3d — 3-vector (fwd; ptr params)
struct fsmEVENT_PARAM_BASE; // fsm — event param base (fwd; ptr)
struct fsmCB_POS_DIR_PAR;   // fsm — pos/dir callback param (fwd; Transport arg)
struct fsmHO_INFO;          // fsm/fsmHO_INFO.h — hideout-transform info (fwd; GetFsmInfo out)
struct mdlITRC_BASE;        // mdl — interactive transform ref component (fwd; GetCurrentItrcHO ret)
struct aiBRAIN;             // ai08 — owning brain (fwd; ctor arg)

struct aiBODY : aiBODY_IFACE {                                     // base ends at 0x20
    // 0x??? (??0aiBODY@@QAA@PAVaiBRAIN@@@Z) — construct the FULL body module for the owning brain.
    aiBODY(aiBRAIN *brain);

    propFSM             *bodyFsm;          // 0x20 body property FSM
    usePROCESSOR        *useProcessor;     // 0x24 smart-object use processor
    float                timeUncontrolled; // 0x28 seconds spent uncontrolled (-1 when controllable)
    float                timeFalling;      // 0x2C seconds spent falling
    const ctrlPROP_LIST *propListCache;    // 0x30 cached property bit-list
    mdlINFO_BASE        *infoBodyMdl;      // 0x34 body model info

    // 0x832AB918 (?EnableCollision@aiBODY@@UAAX_N@Z) — virtual: toggle the body's sphere-refine
    // collision on/off (clears the 0x80 "disabled" bit in the refine state when `enable`).
    // REVERSED: aiBODY__EnableCollision.cpp.
    void EnableCollision(bool enable);

    // 0x832AB998 (?IsAimedForMelee@aiBODY@@UBA_NXZ) — virtual, const: the body's model-state cache
    // reports procedural aim is not clamped (melee-aligned). REVERSED: aiBODY__IsAimedForMelee.cpp.
    bool IsAimedForMelee() const;

    // 0x832AB9E0 (?GetShootingResponse@aiBODY@@UBAHXZ) — virtual, const: 1 if the shooting anim is
    // active, 2 if shooting anim is disabled, else 0. REVERSED: aiBODY__GetShootingResponse.cpp.
    int GetShootingResponse() const;

    // 0x832ABAC0 (?GetHOPointsNearBody@aiBODY@@UBAX...) — virtual, const: collect the hideout points
    // near this body for `hideout`/`target` into `result`, filling `info` with the attacker/target
    // entity+FSM. REVERSED: aiBODY__GetHOPointsNearBody.cpp.
    void GetHOPointsNearBody(hoOBJECT *hideout, aiWATCHER *target, bool preferCenter,
                             dsVECTOR<dsSHARED_PTR<mdlITRC_HO, 0, Deleter<mdlITRC_HO> >, 8> &result,
                             aiBODY_IFACE::aiPTS_NEAR_BODY_ADDITIONAL_INFO &info) const;

    // 0x832ABD38 (?DbgGetRadCdt@aiBODY@@UBAMXZ) — virtual, const: the body's CDT collision radius
    // (from the body FSM's cmd block, or the brain's CDT params). REVERSED: aiBODY__DbgGetRadCdt.cpp.
    float DbgGetRadCdt() const;

    // 0x832ABD88 (?CanUseSmart@aiBODY@@UAA_N...) — virtual: can this body use smart object `smtObj`
    // for entry `action` (usage-condition + availability check). REVERSED: aiBODY__CanUseSmart.cpp.
    bool CanUseSmart(dsSHARED_PTR<smtSMART_OBJ_BASE, 0, Deleter<smtSMART_OBJ_BASE> > &smtObj,
                     const dsPAIR<dsSTRID, dsSTRID> &action);

    // 0x832AC070 (?UpdateUncFall@aiBODY@@IAAXM@Z) — protected: refresh the uncontrolled/falling
    // status flags and the uncontrolled/fall timers this frame. REVERSED: aiBODY__UpdateUncFall.cpp.
    void UpdateUncFall(float dt);

    // 0x832AC2A0 (?SetPar@aiBODY@@UAA_N...M@Z) — virtual: apply a named force-aim tunable
    // (isForceAim / isForceNoAim / isSendNoAimEvent) or forward it to the weapon module.
    // REVERSED: aiBODY__SetPar.cpp.
    bool SetPar(const dsTSTRING<char> &key, float val);

    // 0x832ABFE0-region override — the body's own cached property list (hides the aiBODY_IFACE base
    // default). Body external to this batch (?GetRefPropCache@aiBODY@@UBAABUctrlPROP_LIST@@XZ). boundary.
    const ctrlPROP_LIST *GetRefPropCache() const;
    // (?GetBodyInfo@aiBODY@@UAAPAVmdlINFO_BASE@@XZ) — the body's own model info. boundary.
    mdlINFO_BASE *GetBodyInfo();

    // --- FSM event-forwarding overrides reversed in this batch (src/ws/ai/aiBODY__*.cpp) ---

    // 0x832AC478 — post the named FSM event with no parameter (routes to the held FSM if suspended,
    // else the active FSM). Returns true (always handled).
    bool SendFsmEventByName(const dsSTRID &idEvent);
    // 0x832AC4E0 — post the named FSM event carrying `par`.
    bool SendFsmEventByName(const dsSTRID &idEvent, const fsmEVENT_PARAM_BASE &par);

    // 0x832AC540 — per-frame body step: propagate the mind's "locked" flag to the use processor,
    // run the base body step, then update the uncontrolled/falling timers.
    void ProcessFrame(float dt);

    // 0x832AC7F0 — post a locomotion "set" (pos-goal) event with the goal position and whether it is
    // the last path point; the goal direction defaults to +Y.
    bool SendFsmEventPosGoal(const m3dV &posGoal, bool isLastPoint);

    // 0x832AB5D0 (?Transport@aiBODY@@UAAXABUm3dV@@@Z) — virtual: teleport the body to world position
    // `pos`, preserving its current forward/up facing (queried from the instance).
    // REVERSED: src/ws/ai/aiBODY__Transport.cpp.
    void Transport(const m3dV &pos);

    // 0x832AB6A0 (?SendAlertEvents@aiBODY@@UAAXUm3dV@@@Z) — virtual: fire the body FSM's positional
    // aim event toward `posAlert`, then the named "alert" FSM event.
    // REVERSED: src/ws/ai/aiBODY__SendAlertEvents.cpp.
    void SendAlertEvents(m3dV posAlert);

    // 0x832AC928 — fire the FSM callback for the "reset" event, forwarding the transport param.
    void Transport(fsmCB_POS_DIR_PAR &par);

    // 0x832AC978 — read the current upper-body aiming direction into `dir` via the FSM's
    // "get_cur_aiming" callback; returns true when `dir` is non-null.
    bool GetUpperBodyDir(m3dV *dir) const;

    // 0x832AC9F0 — restore normal gravity on the body (timed no-op scope in this build).
    void SetNormalGravity(const m3dV *grav);

    // 0x832ACA80 — fire the aiming "set" callback with the given aim position (dir=zero, identity
    // matrix param); returns true.
    bool SendFsmEventPosAim(const m3dV &posAim);

    // 0x832ACB20 — attach this body's FSM to smart object `smtObj` at entry action `action`; no-op
    // when the action id is invalid. Returns true when the attach was issued.
    bool AttachToSmart(dsSHARED_PTR<smtSMART_OBJ_BASE, 0, Deleter<smtSMART_OBJ_BASE> > &smtObj,
                       const dsPAIR<dsSTRID, dsSTRID> *action, bool isStreamingWait);

    // 0x832ACDB0 — current hideout ITRC (by value). Selects the HO itrc when it is a live hideout,
    // else the common itrc; empty unless the chosen itrc passes the `filter`/IsValid gate.
    dsSHARED_PTR<mdlITRC_BASE, 0, Deleter<mdlITRC_BASE> > GetCurrentItrcHO(aiBODY_GET_ITRCHO filter) const;

    // 0x832AD398 — enter zero-gravity: on a coin-flip, if the body is in a hideout corner, nudge the
    // FSM up (left corner) or down (right corner).
    void SetZeroGravity(bool wasInHo);

    // 0x832AD500 — copy the use processor's hideout info into `infoHO`; returns true only when the
    // HO itrc exists and reports itself as an active hideout.
    bool GetFsmInfo(fsmHO_INFO &infoHO);

    // 0x832AD5C8 (?ProcessInitLevel@aiBODY@@UAAXXZ) — virtual: per-level body init. Subscribe the
    // brain's OnCdtProcessed handler to the body FSM's "cdt_processed" event, derive the body's cdt
    // ability bits from the species descriptor abilities + current body state, and (for
    // tower-man / smart-death units) disable hit reactions on the body FSM. REVERSED:
    // aiBODY__ProcessInitLevel.cpp.
    void ProcessInitLevel();
};
