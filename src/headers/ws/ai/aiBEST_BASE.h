#pragma once
#include "../ds/dsTSTRING.h"
// ws-engine ai08: base class for a brain's "best behaviour" selection module.
// DB-verified layout (types_members aiBEST_BASE): __vftable@0 — size 4.

struct aiBEST_BASE_vtbl;
struct m3dV; // ws m3d — 3-vector (fwd, boundary)
struct aiVISION_IFACE; // ai08 — brain vision interface (fwd, boundary)
struct bhpSNS_PAR; // bhp — "stay & notify" behaviour params (fwd, boundary)
struct bhpSMART_PAR; // bhp — smart-object behaviour params (fwd, boundary)
struct bhpPARAMS;  // bhp — base behaviour params (fwd, boundary)
struct aiBRAIN;    // ai08 — owning brain (fwd, ConvertToFull arg)
struct entENTITY;  // ent subsystem — game entity (fwd, NoticeGrenade arg)  boundary

struct aiBEST_BASE {
    aiBEST_BASE_vtbl *__vftable; // 0x00

    // vtbl 0xA4 / 0xA8 — pre-/post-process a nav build-path request on the module's owned
    // behaviours. Used by aiBRAIN::PreProcessBuildPath / PostProcessBuildPath.  boundary.
    void PreProcessBuildPath();
    void PostProcessBuildPath();

    // vtbl slots 35/36 — relay a grenade notice/termination to the owned behaviours. Used by
    // aiPERC::UpdateGrenadeAvoidance / aiPERC::NoticeGrenadeTerm.  boundary (body external).
    void NoticeGrenade(entENTITY *grenade);
    void NoticeGrenadeTerm(entENTITY *grenade);

    // Virtual — the selected behaviour handles a physics object passing near `pos`; returns true if
    // it consumed the notice (suppressing the perception module's own reaction). Used by
    // aiPERC::NoticeFlyObject.  boundary (body external to this batch).
    bool NoticeFlyObject(const m3dV &pos);

    // 0x83178AF8 (?GetString@aiBEST_BASE@@UBA?AV?$dsTSTRING@D@@ABV2@@Z) — virtual, const.
    // Base default: ignore `desc` and return the shared empty string (sret).
    virtual dsTSTRING<char> GetString(const dsTSTRING<char> &desc) const;

    // The behaviour "context" id of the currently-selected behaviour. Used by aiBRAIN::implSetTimer
    // to tag the timer it arms.  boundary (body external to this batch).
    int GetBhvContext();

    // True when this module currently owns the behaviour with id `bid` (an aiBID value; passed as
    // int to avoid pulling the aiBID enum's boundary header). Used by aiCON_CB::GetAICount.
    // boundary (body external to this batch).
    bool HasBHV(int bid);

    // Virtual — write the position the unit currently wants to reach into `out`. Used by
    // aiWATCHER_BRAIN::GetWannaBePos.  boundary (body external to this batch).
    void GetWannaBePos(m3dV *out);

    // Virtual (vtbl+0x54) — best-behaviour hook invoked while the brain's vision arm computes its
    // eye sample (aiVISION_IFACE::CalcPosEye); lets the active behaviour adjust brain state before
    // the eye position is used. Inferred vtbl slot from the CalcPosEye call site.  boundary.
    void ProcessCalcPosEye(aiVISION_IFACE *iface);

    // Notify the best-behaviour module that the unit took `amount` of damage. Used by
    // aiMIND::NoticeWounded.  boundary (body external to this batch).
    void NoticeWounded(float amount);

    // Find the currently-owned behaviour of id `bid` (an aiBID value; int to avoid the boundary
    // enum). Returns the behaviour object (an aiBHV_BASE*), null when absent. Used by the ai08
    // debug path exporters. boundary — body external to this batch.
    void *FindBHV_2(int bid);

    // Return the currently-selected "major" behaviour object (a bhvBEHAV*), null when none. `flags`
    // selects the query variant (0 = default). Used by aiDEBUG::RenderOtherTempGroup. Declared to
    // return the bhvBEHAV boundary type. boundary — body external to this batch.
    struct bhvBEHAV *GetMajorBehavior(int flags);

    // ?UpdateTimers@aiBEST_BASE@@UAAXM@Z — virtual: advance this module's timers by `dt` seconds.
    // Used by aiMIND::UpdateTimers.  boundary.
    void UpdateTimers(float dt);

    // ?AddBhvFuncCall@aiBEST@@UAA_NABV?$dsTSTRING@D@@0@Z — virtual (vtbl+0xD8): offer an SSL
    // ordered function call (On_<bhvPrefix>_<nameFunc>) to the owned behaviours; true if any
    // accepted. Base slot; overridden in aiBEST. Reached via aiBRAIN::implCallBhvFuncOnce.
    virtual bool AddBhvFuncCall(const dsTSTRING<char> &bhvPrefix, const dsTSTRING<char> &nameFunc);

    // ?ConvertToFull@aiBEST_BASE@@UAAXPAVaiBRAIN@@@Z @ 0x83297B88 — virtual: replace the brain's
    // (possibly bare/singleton) best-behaviour module with a freshly-constructed full aiBEST, swapping
    // it into pBrain->mind.best and releasing the previous module. REVERSED: aiBEST_BASE__ConvertToFull.cpp.
    virtual void ConvertToFull(aiBRAIN *pBrain);

    // 0x83174468 (?NoticeBeforeDeath@aiBEST_BASE@@UAAXXZ) — virtual: notify the module that its brain
    // is about to die (base is a no-op; overridden in aiBEST/bhvBEHAV). Used by aiDIE::Start.  boundary.
    void NoticeBeforeDeath();

    // vtbl — force-clear the best-behaviour module (teardown). Used by aiMIND::ForcedClear.  boundary.
    void ForcedClear();
    // vtbl — notify the module the unit was teleported. Used by aiBRAIN::Transport.  boundary.
    void NoticeTransported();
    // vtbl — the currently-selected behaviour object (a bhvBEHAV*), null when none. Used by
    // aiBRAIN::ProcessTimerPool to route a fired timer.  boundary.
    struct bhvBEHAV *FindBHV();
    // vtbl — post-body per-frame best-behaviour update. Used by aiMIND::ProcessFrameAfterBody.  boundary.
    void ProcessFrameAfterBody();
    // vtbl — fast (sparsed) post-body path. Used by aiMIND::ProcessFrameAfterBody.  boundary.
    void FastPFAfterBody();
    // vtbl — pick the initial behaviour after per-level setup. Used by aiMIND::SetupInitial.  boundary.
    void SetupInitialBehavior();
    // vtbl 0x0C — pre-body per-frame best-behaviour update. Used by aiMIND::ProcessFrameBeforeBody.  boundary.
    void ProcessFrameBeforeBody();
    // vtbl 0x14 — fast (sparsed) pre-body path. Used by aiMIND::ProcessFrameBeforeBody.  boundary.
    void FastPFBeforeBody();

    // Command this module into a behaviour described by `params`, tagged with descriptor name
    // `name`. Used by aiTASK_DATA_FN::AFRestart (@TaskSys_Wait_AF@).  boundary (body external).
    void SetBehaviorByParams(bhpSNS_PAR *params, const char *name);

    // Overload reached by aiBRAIN::TryToUseAnimDeathSmt: command this module into a smart-object
    // behaviour described by `params`, tagged `name`; returns whether it was accepted.  boundary.
    bool SetBehaviorByParams(bhpSMART_PAR *params, const char *name);

    // ?SetBehaviorByParams@aiBEST_BASE@@UAA_NPAVbhpPARAMS@@PBD@Z — virtual: command this module into a
    // behaviour described by `params`, tagged `name`; returns whether accepted. Used by implUseTurret.
    bool SetBehaviorByParams(bhpPARAMS *params, const char *name);

    // 0x83174488 (?PushBehaviorByParams@aiBEST_BASE@@UAA_NPAVbhpPARAMS@@HPBD@Z) — virtual (vtbl+0xB0):
    // push a behaviour described by `params` (isSubBehav flag `sub`), tagged with descriptor `name`;
    // returns whether it was accepted. Used by aiBRAIN::implAvoidDanger / implAvoidGrenade. boundary.
    bool PushBehaviorByParams(bhpPARAMS *params, int sub, const char *name);

    // 0x... (?NoticeBodyUncontrolled@aiBEST_BASE@@UAAXXZ) — virtual (vtbl+0x70): notify the module
    // the body just became uncontrolled. Used by aiBODY::UpdateUncFall.  boundary — body external.
    void NoticeBodyUncontrolled();
    // 0x... (?NoticeBodyControllable@aiBEST_BASE@@UAAXM@Z) — virtual (vtbl+0x6C): notify the module
    // the body regained control. The DB mangling carries a float param the UpdateUncFall call site
    // leaves in fp1 as a residual (the decompiler drops it); declared arg-less to match the call.
    // Used by aiBODY::UpdateUncFall.  boundary — body external.
    void NoticeBodyControllable();

    // vtbl — finish (terminate) every owned behaviour of behaviour-id `bid` (an aiBID value; passed as
    // int to avoid pulling the aiBID enum's boundary header). Used by aiBRAIN::Transport/implMoveToHO.
    // boundary — body external to this batch.
    void FinishBehavs(int bid);
    // vtbl — clear the module's behaviour-to-behaviour ("B2B") parcel hand-offs. Used by
    // aiBRAIN::Transport.  boundary — body external to this batch.
    void ClearB2BParcels();

    // vtbl+0xB4 (?GetTopBehavior@aiBEST_BASE@@...) — the behaviour currently on top of the module's
    // stack (null when none). Used by aiBRAIN::implHasAnyAvailableStrike.  boundary — body external.
    struct bhvBEHAV *GetTopBehavior();
};
