#pragma once
#include "aiBRAIN_IFACE.h"
#include "aiMIND.h"
#include "aiNAMES.h"
#include "aiCMD2BODY.h"
#include "aiTIMER_POOL.h"
#include "aiTRASH_BIN.h"
#include "aiDEBUG.h"
#include "../ds/dsSHARED_PTR.h"
#include "../ds/dsFLAGS.h"
#include "../ds/dsVECTOR.h"
#include "../m3d/m3dV.h"

struct m3dMATR; // m3dMATR.h — 4x3 affine matrix   boundary (fwd)
struct navSYS;  // nav subsystem                    boundary (fwd)
struct mdlCDT_REFINE_FLOOR_AI; // mdl/mdlCDT_REFINE_FLOOR_AI.h — AI floor refine query   boundary (fwd)
struct msgADDR; // msg/msgADDR.h — message endpoint  boundary (fwd; PreProcessMsg arg)
struct fioFILE; // fio/fioFILE.h — chunked stream    boundary (fwd; Write/ReadSaveDataFarm arg)
enum SSL_COMMON_BACKOFF; // ai08 — ssl common back-off reason flags   boundary
struct dsDATA;    // ds/dsDATA.h — type-erased script value   boundary (fwd; full type in .cpp)
struct sslOBJ_REF;// ssl/sslOBJ_REF.h — script object reference  boundary (fwd; full type in .cpp)
struct sslCB_HOST;         // ssl/sslCB_HOST.h — script callback host  boundary (fwd)
struct dsVECTOR_PARAM_LIST;// ds/dsVECTOR_PARAM_LIST.h — script param list  boundary (fwd)
namespace ds_data { template<class T> struct REF_TYPE; } // ds/REF_TYPE.h  boundary (fwd)
// ws-engine ai08: a fully-realised per-unit AI brain.
// DB-verified layout (types_members aiBRAIN) — size 0x1850. Only the members the batched methods
// reach (mind, st) are typed; the rest are byte-accurate opaque placeholders keyed to the DB
// member offsets so the layout stays exact (their bodies are the next frontier).

struct aiBODY_IFACE; // aiWATCHER_BRAIN_boundaries.h — body interface  boundary
struct aiSTATUS;     // aiWATCHER_BRAIN_boundaries.h — per-brain status block boundary (pos read at raw +0x110 by AlertTeam's caller)
struct m3dV;         // ws m3d — 3-vector
struct dsDATA;       // ds type-erased value (full def ds/dsDATA.h)   boundary (fwd)
struct sslOBJ_REF;   // ssl script object reference (full def ssl/sslOBJ_REF.h)  boundary (fwd)
struct dsSTRID;      // ds interned string id (full def ds/dsSTRID.h) boundary (fwd)
struct iaIACTOR;     // ws — interactive actor (full def ws/ia/iaIACTOR.h)  boundary (fwd)

struct aiBRAIN : aiBRAIN_IFACE {                          // base ends at 0x1C0
    dsSHARED_PTR<aiBODY_IFACE, 0, Deleter<aiBODY_IFACE> > body; // 0x1C0 body interface (wpn holder)
    aiMIND        mind;                                   // 0x1C8
    aiCMD2BODY    cmd;                                     // 0x264 per-frame command block
    dsSHARED_PTR<aiSTATUS, 0, Deleter<aiSTATUS> > st;     // 0x2CC status (pos floats at +0x110..0x118)
    aiTIMER_POOL  timers;                                 // 0x2D4 countdown timer pool
    aiNAMES       names;                                  // 0x2EC debug/instance name pointers
    aiTRASH_BIN   bin;                                    // 0x2F8 deferred-destruction bin
    aiDEBUG       dbg;                                     // 0x324 per-brain visual-debug block (0x152C)

    // 0x8316FCC4-region call — write the brain's current world-space velocity into `out`.
    // boundary (body external to this batch).
    void GetSpeed(m3dV *out);

    // 0x832AC8B0 (?GetFloorRefine@aiBRAIN@@UBAPAVcdtREFINE@@XZ) — the process-wide AI floor
    // collision-refine query object, retargeted to this brain's animated instance.
    // REVERSED: src/ws/ai/aiBRAIN__GetFloorRefine.cpp.
    mdlCDT_REFINE_FLOOR_AI *GetFloorRefine() const;

    // 0x832B4028 (?OnEnemyDamaged@aiBRAIN@@QAAXMH@Z) — report that this brain dealt `dmg` damage
    // (with `killed` kills) to its current enemy, firing the SSL OnEnemyDamaged event. Used by
    // aiPERC::UpdateVictimDamaged.  boundary (body external to this batch).
    void OnEnemyDamaged(float dmg, int killed);

    // 0x83255950 (?OnEnterHO@aiBRAIN@@QAAXXZ) — the body just entered a hideout; run the brain-side
    // enter-HO bookkeeping/SSL event. Used by aiHO_SYS::NoticeEnteredHO. boundary (body external).
    void OnEnterHO();
    // 0x83255A30 (?OnLeaveHO@aiBRAIN@@QAAXXZ) — the body just left its hideout. Used by
    // aiHO_SYS::ProcessLeaveHO. boundary (body external).
    void OnLeaveHO();

    // 0x8317A908 (?UpdateBodyCS@aiBRAIN@@QAAXXZ) — refresh the status coordinate-system block from
    // the body/instance transforms. REVERSED: aiBRAIN__UpdateBodyCS.cpp.
    void UpdateBodyCS();
    // 0x8317AA18 (?ProcessINIT_LEVEL@aiBRAIN@@UAAHXZ) — virtual: per-level init (register with the
    // planner, init body/status/mind). REVERSED: aiBRAIN__ProcessINIT_LEVEL.cpp.
    int  ProcessINIT_LEVEL();
    // 0x8317AC00 (?implSetTimer@aiBRAIN@@UAAHM_N@Z) — virtual: arm a brain timer of `duration`
    // seconds (cyclic if isCycle), tagging it with the current best-behaviour context.
    // REVERSED: aiBRAIN__implSetTimer.cpp.
    int  implSetTimer(float duration, bool isCycle);

    // 0x832ADC60 (?GetFaceParam@aiBRAIN@@UBAXPAUm3dV@@0@Z) — virtual const: compute the brain's face
    // direction/position, correcting for high-hideout offsets or falling back to weapon-tip / instance
    // transforms. `dir`/`pos` may be null. REVERSED: aiBRAIN__GetFaceParam.cpp.
    void GetFaceParam(m3dV *dir, m3dV *pos) const;
    // 0x832AE088 (?CorrectFaceByHighHO@aiBRAIN@@UBAXPAUm3dV@@0@Z) — virtual const: shift `pos` by the
    // descriptor's horizontal/vertical high-hideout face offsets when the self-peeker is in a high
    // hideout (non-mini-zoom). REVERSED: aiBRAIN__CorrectFaceByHighHO.cpp.
    void CorrectFaceByHighHO(m3dV *dir, m3dV *pos) const;

    // --- saber2 aiBRAIN virtual-override batch (0x832AD770-0x832AE3D8). REVERSED this wave. ---
    // 0x832AD770 (?PreProcessMsg@aiBRAIN@@UAAHHPAXPAVmsgADDR@@@Z) — virtual pre-dispatch hook: run
    // OnDestroy on TERM, forward to the base, and on INIT/spawn messages init subsystems / reset the
    // instance CDT layer. REVERSED: aiBRAIN__PreProcessMsg.cpp.
    int  PreProcessMsg(int msg, void *pInfo, struct msgADDR *pSend);
    // 0x832AD810 (?PreProcessINIT@aiBRAIN@@UAAXXZ) — virtual: build the brain subsystems then run the
    // body's pre-process init. REVERSED: aiBRAIN__PreProcessINIT.cpp.
    void PreProcessINIT();
    // 0x832AD870 / 0x832AD8C8 (?implHide@ / ?implShow@aiBRAIN@@UAAXXZ) — virtual: hide/show the brain
    // (base entity hide/show + toggle the low suspend-state bit through OnChangeMode). REVERSED.
    void implHide();
    void implShow();
    // 0x832ADAB8 (?GetEnemyType_SSL@aiBRAIN@@QBAMXZ) — SSL helper: classify the current enemy
    // (none=0, neutral=0.5, other-team=3, player=2, brain/entity=1, else -1). REVERSED.
    float GetEnemyType_SSL() const;
    // 0x832ADD48 / 0x832ADDB0 (?PreProcessBuildPath@ / ?PostProcessBuildPath@aiBRAIN@@UAAXPAVnavSYS@@@Z)
    // — virtual: when `ns` is this brain's own nav system, forward pre/post build-path to the best
    // module. REVERSED: aiBRAIN__PreProcessBuildPath.cpp / aiBRAIN__PostProcessBuildPath.cpp.
    void PreProcessBuildPath(struct navSYS *ns);
    void PostProcessBuildPath(struct navSYS *ns);
    // 0x832ADED0 / 0x832ADF98 (?WriteSaveDataFarm@ / ?ReadSaveDataFarm@aiBRAIN@@UAAXPAVfioFILE@@@Z) —
    // virtual: persist/restore the "died on farm" marker chunk (0x122). REVERSED.
    void WriteSaveDataFarm(struct fioFILE *stream);
    void ReadSaveDataFarm(struct fioFILE *stream);
    // 0x832AE030 (?implSetLOD@aiBRAIN@@UAAXH@Z) — virtual: forward LOD `idx` to the base entity and
    // the weapon module. REVERSED: aiBRAIN__implSetLOD.cpp.
    void implSetLOD(int idx);
    // 0x832AE3D8 (?GetHealthPercent@aiBRAIN@@QBAMXZ) — the brain's current health as a 0..1 fraction
    // (0 when dead). REVERSED: aiBRAIN__GetHealthPercent.cpp.
    float GetHealthPercent() const;

    // 0x83176270 (?SetupSubSystems@aiBRAIN@@MAAXXZ) — protected virtual: build the brain's body
    // and mind subsystems. REVERSED: aiBRAIN__SetupSubSystems.cpp.
    void SetupSubSystems();
    // Body/mind construction helpers invoked by SetupSubSystems. Bodies external. boundary.
    void SetupBody();
    void SetupMind();

    // 0x831762C0 (?ProcessTERM@aiBRAIN@@UAAXXZ) — virtual: terminate the brain (instant teardown,
    // base ProcessTERM, drop SSL cross-refs). REVERSED: aiBRAIN__ProcessTERM.cpp.
    void ProcessTERM();
    // 0x831776D0 (?TermInstant@aiBRAIN@@MAAXXZ) — protected virtual: immediate teardown when the
    // brain is still active. REVERSED: aiBRAIN__TermInstant.cpp.
    void TermInstant();
    // 0x83178900 (?ProcessINITAnyHealth@aiBRAIN@@UAAHXZ) — virtual: init the brain regardless of
    // health (names, status flags, combat/mind/cmd defaults). REVERSED: aiBRAIN__ProcessINITAnyHealth.cpp.
    int ProcessINITAnyHealth();
    // 0x831789D8 (?CopyDataFromDescToStatus@aiBRAIN@@IAAXXZ) — protected: copy descriptor-driven
    // flags/values into the status block. REVERSED: aiBRAIN__CopyDataFromDescToStatus.cpp.
    void CopyDataFromDescToStatus();
    // 0x83179790 (?RemoveSSLCrossRefs@aiBRAIN@@MAAXXZ) — protected virtual: clear any gsGAME_TIMER
    // callbacks held by this brain's SSL object variables. REVERSED: aiBRAIN__RemoveSSLCrossRefs.cpp.
    void RemoveSSLCrossRefs();

    // 0x83179CA8 (?ProcessINIT@aiBRAIN@@UAAHPAX@Z) — virtual: per-spawn brain init. Runs the base
    // entity init, then dispatches to the live- or dead-spawn mind path. REVERSED:
    // aiBRAIN__ProcessINIT.cpp.
    int ProcessINIT(void *pInfo);
    // 0x83179700 (?ProcessINITDead@aiBRAIN@@QAAXXZ) — kill the brain (Die("")) and run the dead
    // mind init. REVERSED: aiBRAIN__ProcessINITDead.cpp.
    void ProcessINITDead();
    // 0x83179F98 (?implKillTimer@aiBRAIN@@UAAXH@Z) — virtual: remove timer `timerID`, logging a
    // diagnostic when it was already absent. REVERSED: aiBRAIN__implKillTimer.cpp.
    void implKillTimer(int timerID);
    // 0x8317A870 (?GetBodyCS@aiBRAIN@@IAAXAAUm3dV@@00@Z) — protected: fetch the body's world-space
    // coordinate system (pos/dir/up). REVERSED: aiBRAIN__GetBodyCS.cpp.
    void GetBodyCS(m3dV &pos, m3dV &dir, m3dV &dirUp);
    // 0x832AE528 (?Die@aiBRAIN@@UAAXABV?$dsTSTRING@D@@@Z) — virtual: kill the brain with reason
    // string `reason` by locating the brain's fireable property and delegating to its scripted-death
    // handler. REVERSED: aiBRAIN__Die.cpp.
    void Die(const dsTSTRING<char> &reason);

    // 0x832A0A90 (?OnAlert@aiBRAIN@@UAAHW4AI_ALERT@@@Z) — virtual: raise a `kind` alert on this
    // brain, dispatching the matching per-species "OnAlert(...)" SSL event. Used by
    // aiMIND::ForceDelayedCalls. REVERSED: src/ws/ai/aiBRAIN__OnAlert.cpp.
    int OnAlert(AI_ALERT kind);
    // 0x832A06C8 (?implSetSenses@aiBRAIN@@UAAXABV?$dsTSTRING@D@@@Z) — virtual: switch the perception
    // senses preset (idle/battle/default/disabled) named by `id`. REVERSED: src/ws/ai/aiBRAIN__implSetSenses.cpp.
    void implSetSenses(const dsTSTRING<char> &id);
    // 0x832A1880 (?SetupIdleSubSystems@aiBRAIN@@MAAXXZ) — protected virtual: install the idle-set of
    // mind subsystems. boundary — body external to this batch.
    void SetupIdleSubSystems();

    // 0x832AD868 (?ResetAllSparsings@aiBRAIN@@QAAXXZ) — reset every sparsed-update (LOD skip) flag
    // on this brain. Used by aiMIND scripted-enemy transitions. boundary — body external.
    void ResetAllSparsings();

    // 0x83255F18 (?IsSSLCommonBackOff@aiBRAIN@@MBA_N...) — protected virtual: reject an SSL call
    // named `name` when the brain is in a common back-off state (`flags` selects which checks).
    // boundary — body external to this batch.
    bool IsSSLCommonBackOff(const dsTSTRING<char> &name, dsFLAGS<SSL_COMMON_BACKOFF, int> flags) const;

    // 0x83173418 (?implSetTeamName@aiBRAIN@@UAAX...) — virtual: assign the brain's team from the
    // named team `sTeam` (must be gs-registered). REVERSED: aiBRAIN__implSetTeamName.cpp.
    void implSetTeamName(const dsTSTRING<char> &sTeam);
    // 0x83173630 (?implGetTeamName@aiBRAIN@@UAA...) — virtual: the brain's team id as a string.
    // REVERSED: aiBRAIN__implGetTeamName.cpp.
    dsTSTRING<char> implGetTeamName();
    // 0x83173698 (?implCountMyNeutralsAroundMe@aiBRAIN@@UAAHM@Z) — virtual: count neutral units
    // within `radius` of this brain. REVERSED: aiBRAIN__implCountMyNeutralsAroundMe.cpp.
    int implCountMyNeutralsAroundMe(float radius);
    // 0x83173790 (?implCountMyNeutralsAroundEnemy@aiBRAIN@@UAAHM@Z) — virtual: count neutral units
    // within `radius` of this brain's enemy. REVERSED: aiBRAIN__implCountMyNeutralsAroundEnemy.cpp.
    int implCountMyNeutralsAroundEnemy(float radius);

    // --- virtuals forwarded through by aiWATCHER_BRAIN's body/nav accessors. Bodies external to
    //     this batch (the next frontier).  boundary ---
    m3dV    GetBodyPosNav() const;              // nav-space body position (sret)
    void    GetBodyMatrNav(m3dMATR &matr) const;// nav-space body matrix
    // 0x8317xxxx (?GetCdtParams@aiBRAIN@@...) — fetch the brain's collision-detection tuning block
    // (radii/heights used by the CDT debug cylinder). Body external to this batch. boundary.
    const struct mdlCDT_SETTINGS *GetCdtParams() const;

    m3dV    GetPos() const;                      // world body position (sret)
    m3dV    GetDir() const;                      // world body forward (sret)
    m3dV    GetDirUp() const;                    // world body up (sret)
    bool    IsDead() const;                      // brain is dead
    bool    HaveNavSystem() const;               // brain owns a nav system (boundary — body external)
    void    RemoveTimersByContext(unsigned int context); // drop all timers armed for a bhv context (boundary)
    navSYS *GetNavSystem() const;                // owning nav system
    // 0x832ABB88 (?GetMinHeightFlesh@aiBRAIN@@UBAMXZ) — virtual, const: the brain's min flesh height
    // (from its status peeker watcher, else the game-info default). REVERSED: aiBRAIN__GetMinHeightFlesh.cpp.
    float   GetMinHeightFlesh() const;           // min flesh height above feet (aiDEBUG reticle viz)
    // 0x832ABBF8 (?GetHeight@aiBRAIN@@UBAMXZ) — virtual, const: the brain's full body height (from its
    // status peeker watcher, else the game-info default). REVERSED: aiBRAIN__GetHeight.cpp.
    float   GetHeight() const;
    // 0x832ABE30 (?OnCdtProcessed@aiBRAIN@@UAAXPAUfsmPARAM_CDT@@@Z) — virtual: fold a processed
    // collision-detection result `par` into the status radar/body flags. REVERSED: aiBRAIN__OnCdtProcessed.cpp.
    void    OnCdtProcessed(struct fsmPARAM_CDT *par);

    // ?OnStrikeStart@aiBRAIN@@... — notify the brain that the named strike began firing (fired by
    // aiWEAPON::UpdateTimers when the delayed-OSS window closes). REVERSED: aiBRAIN__OnStrikeStart.cpp.
    void OnStrikeStart(const dsSTRID &name);

    // 0x832B34C0 (?OnStrikeEnd@aiBRAIN@@QAAXABVdsSTRID@@@Z) — notify the brain that the named strike
    // finished firing; dispatches the descriptor "OnStrikeEnd" SSL event with the strike name boxed
    // as its payload (unless the common SSL back-off gate rejects it). REVERSED: aiBRAIN__OnStrikeEnd.cpp.
    void OnStrikeEnd(const dsSTRID &name);

    // 0x832B31A8 (?OnReload@aiBRAIN@@QAAXXZ) — notify the brain that its weapon just reloaded;
    // dispatches the descriptor "OnReload" no-arg SSL event (unless the common SSL back-off gate
    // rejects it). Fired by aiWEAPON::Reload. REVERSED: aiBRAIN__OnReload.cpp.
    void OnReload();

    // --- exclamation-stream callbacks (aiBASS::ProcessStream / InternalPlayEx). Bodies external to
    //     this batch (the next frontier).  boundary ---
    // 0x83... (?OnExPlayed@aiBRAIN@@QAAXABV?$dsTSTRING@D@@@Z) — the named exclamation just played.
    void  OnExPlayed(const dsTSTRING<char> &exName);
    // 0x8324CB28 (?OnExStarted@aiBRAIN@@QAAXABV?$dsTSTRING@D@@M@Z) — the named exclamation started
    // playing, with the given total duration.  boundary.
    void  OnExStarted(const dsTSTRING<char> &exName, float duration);
    // 0x8324CE08 (?OnExCancelled@aiBRAIN@@QAAXABV?$dsTSTRING@D@@@Z) — the named exclamation was cancelled.
    void  OnExCancelled(const dsTSTRING<char> &exName);
    // 0x83... (?GetDistToClosestPlayer@aiBRAIN@@QBAMXZ) — distance from this brain to the nearest player.
    float GetDistToClosestPlayer() const;

    // --- SSL script-function wrappers (aiBRAIN "cmd" batch @ 0x83249A50-0x8324A5C8). Each reads a
    //     per-species SSL function id out of the owned descriptor (spDesc.pointee, an aiBRAIN_DESC —
    //     see aiBRAIN_DESC_sslfunc_boundary.h) and dispatches it through sslObject.CallFunc.
    //     REVERSED: src/ws/ai/aiBRAIN__<Method>.cpp ---
    // 0x83249A50 (?IsEnemyType@aiBRAIN@@QAA_NABV?$dsTSTRING@D@@@Z) — run the "IsEnemyType" script
    // function with team/type name `typeName`; returns its bool result.
    bool IsEnemyType(const dsTSTRING<char> &typeName);
    // 0x83249BC8 (?SetAim@aiBRAIN@@QAAXABV?$dsTSTRING@D@@@Z) — run "SetAim" with target name.
    void SetAim(const dsTSTRING<char> &target);
    // 0x83249D08 (?EndAim@aiBRAIN@@QAAXXZ) — run "EndAim" (no args).
    void EndAim();
    // 0x83249D88 (?cbGetAim@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback: return the
    // brain's current aim name (implGetAim) into `retVal`.
    void cbGetAim(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83249E58 (?SetPar@aiBRAIN@@QAAXABV?$dsTSTRING@D@@M@Z) — run "SetPar" with (name, value).
    void SetPar(const dsTSTRING<char> &name, float value);
    // 0x83249FF8 (?SetSenses@aiBRAIN@@QAAXABV?$dsTSTRING@D@@@Z) — run "SetSenses" with a config name.
    void SetSenses(const dsTSTRING<char> &senses);
    // 0x8324A138 (?ResetAlerts@aiBRAIN@@QAAXXZ) — run "ResetAlerts" (no args).
    void ResetAlerts();
    // 0x8324A1B8 (?SetSniperVision@aiBRAIN@@QAAX_N@Z) — run "SetSniperVision" with an on/off flag.
    void SetSniperVision(bool enable);
    // 0x8324A2A0 (?cbSetSniperVision@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback:
    // read a bool from `argv` and forward to implSetSniperVision.
    void cbSetSniperVision(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x8324A308 (?ForceWar@aiBRAIN@@QAAXXZ) — run "ForceWar" (no args).
    void ForceWar();
    // 0x8324A388 (?ForcePeace@aiBRAIN@@QAAXXZ) — run "ForcePeace" (no args).
    void ForcePeace();
    // 0x8324A408 (?AutoBattle@aiBRAIN@@QAAXXZ) — run "AutoBattle" (no args).
    void AutoBattle();
    // 0x8324A488 (?ShootNow@aiBRAIN@@QAAXABV?$dsTSTRING@D@@@Z) — run "ShootNow" with a target name.
    void ShootNow(const dsTSTRING<char> &target);
    // 0x8324A5C8 (?EnableStrike@aiBRAIN@@QAAXABV?$dsTSTRING@D@@@Z) — run "EnableStrike" with a name.
    void EnableStrike(const dsTSTRING<char> &strike);

    // --- aiBRAIN strike/weapon "cmd" SSL batch (0x8324A708-0x8324B370). Same dispatch shape as the
    //     cmd batch above; ids read from spDesc.pointee (see aiBRAIN_DESC_sslfunc_boundary.h).
    //     REVERSED: src/ws/ai/aiBRAIN__<Method>.cpp ---
    // 0x8324A708 (?DisableStrike@aiBRAIN@@QAAXABV?$dsTSTRING@D@@@Z) — run "DisableStrike" with a name.
    void DisableStrike(const dsTSTRING<char> &strike);
    // 0x8324A848 (?LockStrike@aiBRAIN@@QAAXABV?$dsTSTRING@D@@M@Z) — run "LockStrike" with (name, lockTime).
    void LockStrike(const dsTSTRING<char> &strike, float lockTime);
    // 0x8324A9E8 (?CancelCurStrike@aiBRAIN@@QAAXXZ) — run "CancelCurStrike" (no args).
    void CancelCurStrike();
    // 0x8324AA68 (?DbgStrike@aiBRAIN@@QAAXABV?$dsTSTRING@D@@@Z) — run "DbgStrike" with a name.
    void DbgStrike(const dsTSTRING<char> &strike);
    // 0x8324ABA8 (?SetWpn@aiBRAIN@@QAAXABV?$dsTSTRING@D@@@Z) — run "SetWpn" with a weapon name.
    void SetWpn(const dsTSTRING<char> &weapon);
    // 0x8324ACE8 (?SetSpreadAngle@aiBRAIN@@QAAXM@Z) — run "SetSpreadAngle" with an angle.
    void SetSpreadAngle(float angle);
    // 0x8324ADD0 (?cbSetSpreadAngle@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback:
    // read a float from argv and forward to implSetSpreadAngle.
    void cbSetSpreadAngle(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x8324AE38 (?SetDamageCurve@aiBRAIN@@QAAXABV?$dsTSTRING@D@@@Z) — run "SetDamageCurve" with a name.
    void SetDamageCurve(const dsTSTRING<char> &curve);
    // 0x8324AF78 (?cbAIGetCurWeaponClassName@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL
    // callback: return implAIGetCurWeaponClassName's name into retVal.
    void cbAIGetCurWeaponClassName(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83256DF0 (?AIGetCurWeaponClassName@aiBRAIN@@QAA?AV?$dsTSTRING@D@@XZ) — public getter: run the
    // descriptor's "AIGetCurWeaponClassName" SSL fn (no args) and return its string result.
    dsTSTRING<char> AIGetCurWeaponClassName();
    // 0x83257098 (?GetBHV@aiBRAIN@@QAA?AV?$dsTSTRING@D@@XZ) — public getter: run the descriptor's
    // "GetBHV" SSL fn (no args) and return its string result.
    dsTSTRING<char> GetBHV();
    // 0x8324B048 (?SetContourShootingDist@aiBRAIN@@QAAXMMMM@Z) — run "SetContourShootingDist" with 4 dists.
    void SetContourShootingDist(float d0, float d1, float d2, float d3);
    // 0x8324B238 (?cbSetContourShootingDist@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL
    // callback: read 4 floats from argv and forward to implSetContourShootingDist.
    void cbSetContourShootingDist(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x8324B2C8 (?HasAnyAvailableStrike@aiBRAIN@@QAA_NXZ) — run "HasAnyAvailableStrike"; returns bool.
    bool HasAnyAvailableStrike();

    // --- impl callees of the strike/weapon callbacks above (next frontier — bodies external). boundary ---
    // (?implSetSpreadAngle@aiBRAIN@@...) — set the brain's weapon spread angle on the body.
    void implSetSpreadAngle(float angle);
    // (?implSetContourShootingDist@aiBRAIN@@...) — set the contour-shooting distance band on the body.
    void implSetContourShootingDist(float d0, float d1, float d2, float d3);
    // (?implAIGetCurWeaponClassName@aiBRAIN@@...) — current weapon class name (returns dsTSTRING by value).
    dsTSTRING<char> implAIGetCurWeaponClassName();

    // --- SSL behaviour/approach/exclamation dispatch batch (0x8324BE60-0x8324CCC8). Same descriptor
    //     image (spDesc.pointee — see aiBRAIN_DESC_sslfunc_boundary.h); each cb* getter stores an
    //     impl* result into `retVal`, each dispatcher marshals args and runs the SSL function id.
    //     REVERSED: src/ws/ai/aiBRAIN__<Method>.cpp ---
    // 0x8324BE60 (?cbGetBHV@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback: store the
    // brain's current behaviour name (implGetBHV) into `retVal`.
    void cbGetBHV(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x8324BF30 (?cbGetMajorBHVType@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback:
    // store the brain's major behaviour type name (implGetMajorBHVType) into `retVal`.
    void cbGetMajorBHVType(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x8324C000 (?CallBhvFuncOnce@aiBRAIN@@QAAXABV?$dsTSTRING@D@@0@Z) — run "CallBhvFuncOnce" with
    // (behaviour name, function name).
    void CallBhvFuncOnce(const dsTSTRING<char> &bhvName, const dsTSTRING<char> &funcName);
    // 0x8324C1D8 (?CancelBHVByNameAndStackAbove@aiBRAIN@@QAA_NABV?$dsTSTRING@D@@@Z) — run
    // "CancelBHVByNameAndStackAbove" with a name; returns its bool result.
    bool CancelBHVByNameAndStackAbove(const dsTSTRING<char> &bhvName);
    // 0x8324C350 (?FailBHVByNameAndStackAbove@aiBRAIN@@QAA_NABV?$dsTSTRING@D@@@Z) — run
    // "FailBHVByNameAndStackAbove" with a name; returns its bool result.
    bool FailBHVByNameAndStackAbove(const dsTSTRING<char> &bhvName);
    // 0x8324C4C8 (?SetApproach@aiBRAIN@@QAAXABV?$dsTSTRING@D@@@Z) — run "SetApproach" with a name.
    void SetApproach(const dsTSTRING<char> &approach);
    // 0x8324C608 (?EndApproach@aiBRAIN@@QAAXXZ) — run "EndApproach" (no args).
    void EndApproach();
    // 0x8324C688 (?SetBHVOnEmptyStack@aiBRAIN@@QAA_NXZ) — run "SetBHVOnEmptyStack" (no args); returns
    // its bool result.
    bool SetBHVOnEmptyStack();
    // 0x8324C748 (?cbSetBHVOnEmptyStack@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback:
    // forward (argc, argv) to implSetBHVOnEmptyStack and store its bool result into `retVal`.
    void cbSetBHVOnEmptyStack(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x8324C7D8 (?cbGetNameSpawnedFrom@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback:
    // store the name this brain was spawned from (implGetNameSpawnedFrom) into `retVal`.
    void cbGetNameSpawnedFrom(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x8324C8A8 (?SetViewUpCurve@aiBRAIN@@QAAXABV?$dsTSTRING@D@@@Z) — run "SetViewUpCurve" with a name.
    void SetViewUpCurve(const dsTSTRING<char> &curve);
    // 0x8324C9E8 (?Exclaim@aiBRAIN@@QAAXABV?$dsTSTRING@D@@@Z) — run "Exclaim" with an exclamation name.
    void Exclaim(const dsTSTRING<char> &exName);

    // --- aiBRAIN SSL callback / public-accessor batch @ 0x83257xxx (reversed this wave) ----------
    // Each cb* is an SSL dispatch stub: unbox arg strings/floats out of `argv`, forward to the
    // matching virtual impl*, and (for bool-returning impls) box the result into `retVal`. The
    // Get* accessors run the descriptor's SSL function (no args) and return the string result.
    // 0x83257178 (?GetMajorBHVType@aiBRAIN@@QAA?AV?$dsTSTRING@D@@XZ) — run "GetMajorBHVType" SSL func.
    dsTSTRING<char> GetMajorBHVType();
    // 0x83257258 (?cbCallBhvFuncOnce@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z)
    void cbCallBhvFuncOnce(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83257308 (?cbCancelBHVByNameAndStackAbove@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z)
    void cbCancelBHVByNameAndStackAbove(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x832573C0 (?cbFailBHVByNameAndStackAbove@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z)
    void cbFailBHVByNameAndStackAbove(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83257478 (?cbSetApproach@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z)
    void cbSetApproach(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83257500 (?GetNameSpawnedFrom@aiBRAIN@@QAA?AV?$dsTSTRING@D@@XZ) — run "GetNameSpawnedFrom".
    dsTSTRING<char> GetNameSpawnedFrom();
    // 0x832575E0 (?cbSetViewUpCurve@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z)
    void cbSetViewUpCurve(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83257668 (?cbExclaim@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z)
    void cbExclaim(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x832576F0 (?cbOnExStarted@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z)
    void cbOnExStarted(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83257778 (?cbOnExPlayed@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z)
    void cbOnExPlayed(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83257800 (?cbOnExCancelled@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z)
    void cbOnExCancelled(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83257888 (?cbSetNavSys@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z)
    void cbSetNavSys(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83257910 (?GetNavSys@aiBRAIN@@QAA?AV?$dsTSTRING@D@@XZ) — run "GetNavSys" SSL func.
    dsTSTRING<char> GetNavSys();
    // 0x832579F0 (?cbMoveToPoint@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z)
    void cbMoveToPoint(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);

    // virtual impl* targets of the cb* stubs above (a8_brain.cpp; bodies external to this batch).
    // (?implCallBhvFuncOnce@aiBRAIN@@UAAXABV?$dsTSTRING@D@@0@Z)
    void implCallBhvFuncOnce(const dsTSTRING<char> &bhvName, const dsTSTRING<char> &funcName);
    // (?implCancelBHVByNameAndStackAbove@aiBRAIN@@UAA_NABV?$dsTSTRING@D@@@Z)
    bool implCancelBHVByNameAndStackAbove(const dsTSTRING<char> &bhvName);
    // (?implFailBHVByNameAndStackAbove@aiBRAIN@@UAA_NABV?$dsTSTRING@D@@@Z)
    bool implFailBHVByNameAndStackAbove(const dsTSTRING<char> &bhvName);
    // (?implSetApproach@aiBRAIN@@UAAXABV?$dsTSTRING@D@@@Z)
    void implSetApproach(const dsTSTRING<char> &approach);
    // (?implSetViewUpCurve@aiBRAIN@@UAAXABV?$dsTSTRING@D@@@Z)
    void implSetViewUpCurve(const dsTSTRING<char> &curve);
    // (?implExclaim@aiBRAIN@@UAAXABV?$dsTSTRING@D@@@Z)
    void implExclaim(const dsTSTRING<char> &exName);
    // 0x83299468 (?GetSndSource@aiBRAIN@@UAAXVdsSTRID@@PAViaSND_SOURCE_DATA@@@Z) — virtual: fill
    // `pSrcData`'s start position from this brain's status face position. REVERSED:
    // aiBRAIN__GetSndSource.cpp.
    void GetSndSource(dsSTRID nameObj, struct iaSND_SOURCE_DATA *pSrcData);
    // (?implOnExStarted@aiBRAIN@@UAAXABV?$dsTSTRING@D@@M@Z)
    void implOnExStarted(const dsTSTRING<char> &exName, float when);
    // (?implOnExPlayed@aiBRAIN@@UAAXABV?$dsTSTRING@D@@@Z)
    void implOnExPlayed(const dsTSTRING<char> &exName);
    // (?implOnExCancelled@aiBRAIN@@UAAXABV?$dsTSTRING@D@@@Z)
    void implOnExCancelled(const dsTSTRING<char> &exName);
    // (?implMoveToPoint@aiBRAIN@@UAA_NABV?$dsTSTRING@D@@00_N@Z)
    bool implMoveToPoint(const dsTSTRING<char> &navSys, const dsTSTRING<char> &point,
                         const dsTSTRING<char> &how, bool flag);

    // --- callees of the above (next frontier — bodies external to this batch).  boundary ---
    // 0x8324xxxx (?implGetAim@aiBRAIN@@...) — the brain's current aim name (returns dsTSTRING by value).
    dsTSTRING<char> implGetAim();
    // 0x8324xxxx (?implSetSniperVision@aiBRAIN@@...) — enable/disable sniper vision on the body.
    void implSetSniperVision(bool enable);
    // (?implGetBHV@aiBRAIN@@...) — the brain's current behaviour name (returns dsTSTRING by value).
    dsTSTRING<char> implGetBHV();
    // (?implGetMajorBHVType@aiBRAIN@@...) — the brain's major behaviour type name (dsTSTRING by value).
    dsTSTRING<char> implGetMajorBHVType();
    // (?implGetNameSpawnedFrom@aiBRAIN@@...) — the name this brain was spawned from (dsTSTRING by value).
    dsTSTRING<char> implGetNameSpawnedFrom();
    // (?implSetBHVOnEmptyStack@aiBRAIN@@...) — set the empty-stack behaviour from (argc, argv); bool.
    bool implSetBHVOnEmptyStack(int argc, dsDATA *argv);

    // --- SSL enemy-lock / movement-dist / behaviour-stack dispatch batch (0x8324B388-0x8324BCE8).
    //     Same descriptor image (spDesc.pointee — see aiBRAIN_DESC_sslfunc_boundary.h); each cb*
    //     callback forwards to an impl* body, each dispatcher marshals args and runs the SSL id.
    //     REVERSED: src/ws/ai/aiBRAIN__<Method>.cpp ---
    // 0x8324B388 (?cbHasAnyAvailableStrike@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback:
    // store implHasAnyAvailableStrike's bool into `retVal`.
    void cbHasAnyAvailableStrike(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x8324B418 (?LockEnemy@aiBRAIN@@QAAXM@Z) — run "LockEnemy" with a float argument (no return used).
    void LockEnemy(float arg);
    // 0x8324B500 (?cbLockEnemy@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback: read a
    // float from `argv` and forward to implLockEnemy.
    void cbLockEnemy(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x8324B568 (?SpareEnemy@aiBRAIN@@QAAXM@Z) — run "SpareEnemy" with a float argument (no return used).
    void SpareEnemy(float arg);
    // 0x8324B650 (?cbSpareEnemy@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback: read a
    // float from `argv` and forward to implSpareEnemy.
    void cbSpareEnemy(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x8324B6B8 (?CalcReticleDanger@aiBRAIN@@QAAMXZ) — run "CalcReticleDanger" (no args); return float.
    float CalcReticleDanger();
    // 0x8324B780 (?cbCalcReticleDanger@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback:
    // store implCalcReticleDanger's float into `retVal`.
    void cbCalcReticleDanger(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x8324B810 (?EnemyResetMovementDist@aiBRAIN@@QAAXXZ) — run "EnemyResetMovementDist" (no args).
    void EnemyResetMovementDist();
    // 0x8324B890 (?EnemyGetMovementDist@aiBRAIN@@QAAMXZ) — run "EnemyGetMovementDist" (no args); float.
    float EnemyGetMovementDist();
    // 0x8324B958 (?cbEnemyGetMovementDist@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback:
    // store implEnemyGetMovementDist's float into `retVal`.
    void cbEnemyGetMovementDist(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x8324B9E8 (?ThrowWeaponsAndItems@aiBRAIN@@QAAXXZ) — run "ThrowWeaponsAndItems" (no args).
    void ThrowWeaponsAndItems();
    // 0x8324BA68 (?SetBHV@aiBRAIN@@QAAXABV?$dsTSTRING@D@@@Z) — run "SetBHV" with a behaviour name.
    void SetBHV(const dsTSTRING<char> &bhvName);
    // 0x8324BBA8 (?PushBHV@aiBRAIN@@QAAXABV?$dsTSTRING@D@@@Z) — run "PushBHV" with a behaviour name.
    void PushBHV(const dsTSTRING<char> &bhvName);
    // 0x8324BCE8 (?PushSHOOT@aiBRAIN@@QAA_NABV?$dsTSTRING@D@@@Z) — run "PushSHOOT" with a name; bool.
    bool PushSHOOT(const dsTSTRING<char> &bhvName);

    // --- impl callees of the batch above (next frontier — bodies external). boundary ---
    // 0x832B2CD8 (?implHasAnyAvailableStrike@aiBRAIN@@UAA_NXZ) — virtual: any strike currently available?
    bool implHasAnyAvailableStrike();
    // 0x832B3C38 (?implLockEnemy@aiBRAIN@@UAAXM@Z) — virtual: lock onto the enemy for `arg` seconds.
    void implLockEnemy(float arg);
    // 0x832B6328 (?implSpareEnemy@aiBRAIN@@UAAXM@Z) — virtual: spare the enemy for `arg` seconds.
    void implSpareEnemy(float arg);
    // 0x83248F90 (?implEnemyGetMovementDist@aiBRAIN@@UAAMXZ) — virtual: enemy's accumulated movement dist.
    float implEnemyGetMovementDist();

    // --- SSL event/enemy dispatch batch (0x832490B0-0x83249A34). Each reads a per-species SSL id
    //     out of the owned descriptor (spDesc.pointee — see aiBRAIN_DESC_sslfunc_boundary.h) and
    //     dispatches it through sslObject.CallFunc.  REVERSED: src/ws/ai/aiBRAIN__<Method>.cpp ---
    // 0x832490B0 (?cbTestParamFromAI@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback:
    // return implTestParamFromAI's param list into `retVal`.
    void cbTestParamFromAI(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83249150 (?cbQOT_GetFront@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback:
    // return implQOT_GetFront's param list into `retVal`.
    void cbQOT_GetFront(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x832491F0 (?implCalcReticleDanger@aiBRAIN@@UAAMXZ) — virtual: reticle-based danger heuristic
    // (bumped when the enemy's peeker is shooting near the reticle / in mini-zoom).
    // REVERSED: aiBRAIN__implCalcReticleDanger.cpp.
    float implCalcReticleDanger();
    // 0x83249338 (?OnCreate@aiBRAIN@@QAAXXZ) — run the descriptor's "OnCreate" SSL event (no args).
    void OnCreate();
    // 0x832493B8 (?OnDestroy@aiBRAIN@@QAAXXZ) — run "OnDestroy" (no args).
    void OnDestroy();
    // 0x83249438 (?OnFrameBegin@aiBRAIN@@QAAXXZ) — run "OnFrameBegin" (no args).
    void OnFrameBegin();
    // 0x832494B8 (?OnDbgOut@aiBRAIN@@QAAXXZ) — run "OnDbgOut" (no args).
    void OnDbgOut();
    // 0x83249538 (?IsLive@aiBRAIN@@QAA_NXZ) — run "IsLive" and return its bool result.
    bool IsLive();
    // 0x832495F8 (?cbIsLive@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback: store
    // implIsLive's bool into `retVal`.
    void cbIsLive(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83249688 (?DropWeapon@aiBRAIN@@QAAXXZ) — run "DropWeapon" (no args).
    void DropWeapon();
    // 0x83249708 (?SetEnemy@aiBRAIN@@QAAXABV?$dsTSTRING@D@@@Z) — run "SetEnemy" with an enemy name.
    void SetEnemy(const dsTSTRING<char> &enemyName);
    // 0x83249848 (?EndEnemy@aiBRAIN@@QAAXXZ) — run "EndEnemy" (no args).
    void EndEnemy();
    // 0x832498C8 (?EndEnemySoft@aiBRAIN@@QAAXXZ) — run "EndEnemySoft" (no args).
    void EndEnemySoft();
    // 0x83249948 (?GetEnemy@aiBRAIN@@QAAPAViaIACTOR@@XZ) — run "GetEnemy", resolve the returned script
    // object reference and return the referenced actor (its callback host, as an iaIACTOR*).
    iaIACTOR *GetEnemy();

    // --- impl callees dispatched by the SSL callbacks above (next frontier — bodies external).
    //     boundary ---
    // 0x832491F0-region (?implTestParamFromAI@aiBRAIN@@UAAABV?$REF_TYPE@VdsVECTOR_PARAM_LIST@@@ds_data@@XZ)
    // — virtual: the brain's AI-test parameter list (const ref).
    const ds_data::REF_TYPE<dsVECTOR_PARAM_LIST> &implTestParamFromAI();
    // (?implQOT_GetFront@aiBRAIN@@UAAABV?$REF_TYPE@VdsVECTOR_PARAM_LIST@@@ds_data@@XZ) — virtual:
    // the brain's "quote-of-the-front" parameter list (const ref).
    const ds_data::REF_TYPE<dsVECTOR_PARAM_LIST> &implQOT_GetFront();
    // (?implIsLive@aiBRAIN@@UAA_NXZ) — virtual: is this brain live?
    bool implIsLive();

    // --- SSL nav / parameter ops (a8_brain.cpp, reversed this batch) --------------------------
    // 0x83247C30 (?implSetNavSys@aiBRAIN@@UAAXABV?$dsTSTRING@D@@@Z) — virtual SSL op: localize this
    // brain to the nav-system named `name` (unless a common SSL back-off blocks the "SetNavSys()"
    // call). REVERSED: aiBRAIN__implSetNavSys.cpp.
    void implSetNavSys(const dsTSTRING<char> &name);
    // 0x83247F68 (?implGetNavSys@aiBRAIN@@UAA?AV?$dsTSTRING@D@@XZ) — virtual SSL op: the name of the
    // nav-system this brain's goal module is on (empty string when none). Returns dsTSTRING by
    // value. REVERSED: aiBRAIN__implGetNavSys.cpp.
    dsTSTRING<char> implGetNavSys();
    // 0x83248BA0 (?cbGetEnemy@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback: the
    // brain's current enemy actor's SSL object ref into `retVal`. REVERSED: aiBRAIN__cbGetEnemy.cpp.
    void cbGetEnemy(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83248C68 (?cbGetGroup@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback: the
    // brain's coordinated-group SSL object ref into `retVal`. REVERSED: aiBRAIN__cbGetGroup.cpp.
    void cbGetGroup(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83248D38 (?implSetPar@aiBRAIN@@UAAXABV?$dsTSTRING@D@@M@Z) — virtual SSL op: set named tunable
    // `key` to `val`; body first, then the mind, logging when unrecognized/ambiguous. REVERSED:
    // aiBRAIN__implSetPar.cpp.
    void implSetPar(const dsTSTRING<char> &key, float val);
    // 0x83248DE8 (?implGetFloat@aiBRAIN@@UAAMABV?$dsTSTRING@D@@@Z) — virtual SSL op: read the float
    // SSL parameter named `desc` (0 when unknown). REVERSED: aiBRAIN__implGetFloat.cpp.
    float implGetFloat(const dsTSTRING<char> &desc);
    // 0x83248E70 (?implSetFloat@aiBRAIN@@UAAXABV?$dsTSTRING@D@@M@Z) — virtual SSL op: set the float
    // SSL parameter named `desc` to `value` (always logs "unknown parameter" — no writable float
    // SSL params exist). REVERSED: aiBRAIN__implSetFloat.cpp.
    void implSetFloat(const dsTSTRING<char> &desc, float value);
    // 0x83248EE0 (?implGetInt@aiBRAIN@@UAAHABV?$dsTSTRING@D@@@Z) — virtual SSL op: read the int SSL
    // parameter named `desc` ("squad_members" -> coordinator group size; else 0 + log). REVERSED:
    // aiBRAIN__implGetInt.cpp.
    int implGetInt(const dsTSTRING<char> &desc);

    // 0x832AE630 (?GetFloatSSL@aiBRAIN@@QAAMABVdsSTRID@@@Z) — resolve the interned float SSL param
    // `desc` (health / dist-to-enemy / time-in-HO / etc.) to a value. REVERSED: aiBRAIN__GetFloatSSL.cpp.
    float GetFloatSSL(const dsSTRID &desc);

    // 0x832AED60 (?ProcessFRAME@aiBRAIN@@UAAXXZ) — virtual: the brain's per-frame update — tick the
    // body FSM, run the entity frame, then (once mind setup is done) run the flag/mind/body/timer
    // frame under alive/dead timing counters. REVERSED: aiBRAIN__ProcessFRAME.cpp.
    void ProcessFRAME();
    // 0x832AEF78 (?UpdateTimers@aiBRAIN@@UAAXXZ) — virtual: advance the mind timers, the countdown
    // timer pool, then dispatch fired timers. REVERSED: aiBRAIN__UpdateTimers.cpp.
    void UpdateTimers();
    // 0x832AEBF8 (?ProcessTimerPool@aiBRAIN@@QAAXXZ) — collect timers that fired this frame and route
    // each to the brain (context 0x1000000) or the active behaviour. REVERSED: aiBRAIN__ProcessTimerPool.cpp.
    void ProcessTimerPool();
    // 0x832AEA48 (?UpdateFlags@aiBRAIN@@IAAXXZ) — protected: on a pending ragdoll request, terminate
    // the fireable's RAGDOLL prop; then refresh the mind sparser and optional debug text. REVERSED:
    // aiBRAIN__UpdateFlags.cpp.
    void UpdateFlags();
    // 0x832B3B48 (?implGetEnemy@aiBRAIN@@UAAPAViaIACTOR@@XZ) — virtual: the brain's current enemy
    // actor (null when none). Body external to this batch. boundary.
    iaIACTOR *implGetEnemy();
    // 0x832BB1E8 (?implGetGroup@aiBRAIN@@UAA?AVsslOBJ_REF@@XZ) — virtual: the brain's coordinated
    // group as an SSL object ref (by value). Body external to this batch. boundary.
    sslOBJ_REF implGetGroup();

    // --- SSL nav/exclamation/distance dispatch batch (0x8324CE08-0x8324E0A8). Each reads a
    //     per-species SSL id out of the owned descriptor (spDesc.pointee — see
    //     aiBRAIN_DESC_sslfunc_boundary.h) and dispatches it through sslObject.CallFunc.
    //     REVERSED: src/ws/ai/aiBRAIN__<Method>.cpp ---
    // 0x8324CF48 (?FindExPartner@aiBRAIN@@QAAPAViaIACTOR@@M@Z) — run "FindExPartner" with `range`;
    // resolve the returned SSL object ref to the partner actor (its callback host). REVERSED.
    iaIACTOR *FindExPartner(float range);
    // 0x8324D0B0 (?cbFindExPartner@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback:
    // read a float from `argv`, forward to implFindExPartner, store the actor's ref in `retVal`.
    void cbFindExPartner(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x8324D198 (?SetNavSys@aiBRAIN@@QAAXABV?$dsTSTRING@D@@@Z) — run "SetNavSys" with a nav name.
    void SetNavSys(const dsTSTRING<char> &name);
    // 0x8324D2D8 (?cbGetNavSys@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback: store
    // implGetNavSys's name string into `retVal`.
    void cbGetNavSys(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x8324D3A8 (?MoveToPoint@aiBRAIN@@QAA_NABV?$dsTSTRING@D@@00_N@Z) — run "MoveToPoint" with
    // (point, arrive, style, run); returns its bool result.
    bool MoveToPoint(const dsTSTRING<char> &point, const dsTSTRING<char> &arrive,
                     const dsTSTRING<char> &style, bool run);
    // 0x8324D6A0 (?MoveToHO@aiBRAIN@@QAA_NABV?$dsTSTRING@D@@00_N@Z) — run "MoveToHO" (hold object).
    bool MoveToHO(const dsTSTRING<char> &ho, const dsTSTRING<char> &arrive,
                  const dsTSTRING<char> &style, bool run);
    // 0x8324D998 (?MoveToNavSys@aiBRAIN@@QAA_NABV?$dsTSTRING@D@@00_N@Z) — run "MoveToNavSys".
    bool MoveToNavSys(const dsTSTRING<char> &navSys, const dsTSTRING<char> &arrive,
                      const dsTSTRING<char> &style, bool run);
    // 0x8324DC90 (?HasHOInNavSys@aiBRAIN@@QAA_NXZ) — run "HasHOInNavSys" (no args); bool result.
    bool HasHOInNavSys();
    // 0x8324DD50 (?cbHasHOInNavSys@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback:
    // forward (argc, argv) to implHasHOInNavSys, store the bool into `retVal`.
    void cbHasHOInNavSys(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x8324DDE0 (?ForgetStuck@aiBRAIN@@QAAXXZ) — run "ForgetStuck" (no args).
    void ForgetStuck();
    // 0x8324DE60 (?GetDistToObj@aiBRAIN@@QAAMPAViaIACTOR@@@Z) — run "GetDistToObj" with `obj`'s SSL
    // object ref; returns the float distance.
    float GetDistToObj(iaIACTOR *obj);
    // 0x8324DFC8 (?cbGetDistToObj@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback:
    // resolve `argv`'s object ref, forward to implGetDistToObj, store the float into `retVal`.
    void cbGetDistToObj(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x8324E0A8 (?GetDistToWP@aiBRAIN@@QAAMABV?$dsTSTRING@D@@@Z) — run "GetDistToWP" with a waypoint
    // name; returns the float distance.
    float GetDistToWP(const dsTSTRING<char> &wp);

    // --- SSL AF (approach-force) / QOT ("quote-of-the-front" queue) dispatch batch
    //     (0x83251370-0x83251BC0). Each t*/QOT_* method reads a per-species SSL id out of
    //     spDesc.pointee (aiBRAIN_DESC_sslfunc_boundary.h) and dispatches via sslObject.CallFunc;
    //     each cb* is the native SSL callback that forwards to the matching virtual impl*.
    //     REVERSED: src/ws/ai/aiBRAIN__<Method>.cpp ---
    // 0x83251370 (?tUnlock@aiBRAIN@@QAAXH@Z) — run "tUnlock" SSL func with an int arg.
    void tUnlock(int arg);
    // 0x83251458 (?cbtUnlock@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback: forward
    // the int arg to impltUnlock.
    void cbtUnlock(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x832514C0 (?tSetAFCount@aiBRAIN@@QAAXH@Z) — run "tSetAFCount" SSL func with an int arg.
    void tSetAFCount(int arg);
    // 0x832515A8 (?cbtSetAFCount@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback: forward
    // the int arg to impltSetAFCount.
    void cbtSetAFCount(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83251610 (?tSetAFDelay@aiBRAIN@@QAAXM@Z) — run "tSetAFDelay" SSL func with a float arg.
    void tSetAFDelay(float arg);
    // 0x832516F8 (?cbtSetAFDelay@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback: forward
    // the float arg to impltSetAFDelay.
    void cbtSetAFDelay(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83251760 (?tEnableForceAF@aiBRAIN@@QAAX_N@Z) — run "tEnableForceAF" SSL func with a bool arg.
    void tEnableForceAF(bool arg);
    // 0x83251848 (?cbtEnableForceAF@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback:
    // forward the bool arg to impltEnableForceAF.
    void cbtEnableForceAF(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83260010 (?cbtDo@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback: add a SIMPLE
    // task-fn on the given object (AI_TASK_FN_SIMPLE/AI_TASK_OBJ_OBJ); box the bool result into retVal.
    void cbtDo(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x832600C8 (?cbtDoWait@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback: add a WAIT
    // task-fn on the given object (AI_TASK_FN_WAIT/AI_TASK_OBJ_OBJ); box the bool result into retVal.
    void cbtDoWait(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83260180 (?cbtNDo@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback: add a SIMPLE
    // task-fn on a named object (AI_TASK_FN_SIMPLE/AI_TASK_OBJ_NAME); box the bool result into retVal.
    void cbtNDo(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83260238 (?cbtNDoWait@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback: add a WAIT
    // task-fn on a named object (AI_TASK_FN_WAIT/AI_TASK_OBJ_NAME); box the bool result into retVal.
    void cbtNDoWait(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x832518B0 (?QOT_Reset@aiBRAIN@@QAAXXZ) — run "QOT_Reset" SSL func (no args).
    void QOT_Reset();
    // 0x83251930 (?QOT_ClearFront@aiBRAIN@@QAAXXZ) — run "QOT_ClearFront" SSL func (no args).
    void QOT_ClearFront();
    // 0x832519B0 (?QOT_Size@aiBRAIN@@QAAHXZ) — run "QOT_Size" SSL func; returns the int queue size.
    int QOT_Size();
    // 0x83251A70 (?cbQOT_Size@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback: store
    // implQOT_Size's int into `retVal`.
    void cbQOT_Size(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83251B00 (?QOT_IsEmpty@aiBRAIN@@QAA_NXZ) — run "QOT_IsEmpty" SSL func; returns bool.
    bool QOT_IsEmpty();
    // 0x83251BC0 (?cbQOT_IsEmpty@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback: store
    // implQOT_IsEmpty's bool into `retVal`.
    void cbQOT_IsEmpty(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);

    // --- impl callees of the AF/QOT batch above (virtuals; bodies external — next frontier). boundary ---
    // 0x8325FCF0 (?impltUnlock@aiBRAIN@@UAAXH@Z) — virtual: apply the "tUnlock" op with an int arg.
    void impltUnlock(int arg);
    // 0x8325FD50 (?impltSetAFCount@aiBRAIN@@UAAXH@Z) — virtual: set the AF count.
    void impltSetAFCount(int arg);
    // 0x8325FDB0 (?impltSetAFDelay@aiBRAIN@@UAAXM@Z) — virtual: set the AF delay.
    void impltSetAFDelay(float arg);
    // 0x8325FE18 (?impltEnableForceAF@aiBRAIN@@UAAX_N@Z) — virtual: enable/disable force-AF.
    void impltEnableForceAF(bool arg);
    // vtable slot 0x3D4 (?implQOT_Size@aiBRAIN@@...) — virtual: SSL callback impl; returns queue size.
    int implQOT_Size(int argc, dsDATA *argv);
    // vtable slot (?implQOT_IsEmpty@aiBRAIN@@...) — virtual: SSL callback impl; returns emptiness bool.
    bool implQOT_IsEmpty(int argc, dsDATA *argv);

    // 0x8324CE08 (?OnExCancelled@aiBRAIN@@QAAXABV?$dsTSTRING@D@@@Z) — reversed: run "OnExCancelled"
    // SSL event with the exclamation name. (Declared above at OnExCancelled; body reversed this batch.)

    // --- SSL float-param / smart-object / turret / force-speed dispatch batch
    //     (0x8324E228-0x8324F5F8). Each reads a per-species SSL id out of spDesc.pointee
    //     (aiBRAIN_DESC_sslfunc_boundary.h) and dispatches via sslObject.CallFunc.
    //     REVERSED: src/ws/ai/aiBRAIN__<Method>.cpp ---
    // 0x8324E228 (?GetPathRemain@aiBRAIN@@QAAMXZ) — run "GetPathRemain" (no args); float result.
    float GetPathRemain();
    // 0x8324E2F0 (?cbGetPathRemain@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback:
    // forward (argc, argv) to implGetPathRemain, store the float into `retVal`.
    void cbGetPathRemain(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x8324E380 (?GetDistPlayerToPathEnd@aiBRAIN@@QAAMXZ) — run "GetDistPlayerToPathEnd" (no args);
    // float result.
    float GetDistPlayerToPathEnd();
    // 0x8324E448 (?cbGetDistPlayerToPathEnd@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL
    // callback: forward (argc, argv) to implGetDistPlayerToPathEnd, store the float into `retVal`.
    void cbGetDistPlayerToPathEnd(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x8324E4D8 (?SetForceSpeed@aiBRAIN@@QAA_NABV?$dsTSTRING@D@@@Z) — run "SetForceSpeed" with a
    // speed-preset name; returns its bool result.
    bool SetForceSpeed(const dsTSTRING<char> &speed);
    // 0x8324E650 (?EndForceSpeed@aiBRAIN@@QAAXXZ) — run "EndForceSpeed" (no args).
    void EndForceSpeed();
    // 0x83286D68 (?implForceWar@aiBRAIN@@UAAXXZ) — SSL op: force the "war" battle state (set
    // gen force-war + mind perc-war, fire OnBattleChanged on peace->war). REVERSED:
    // src/ws/ai/aiBRAIN__implForceWar.cpp
    void implForceWar();
    // 0x83286DD0 (?implForcePeace@aiBRAIN@@UAAXXZ) — SSL op: force the "peace" battle state.
    // REVERSED: src/ws/ai/aiBRAIN__implForcePeace.cpp
    void implForcePeace();
    // 0x8324E6D0 (?UseSmartObject@aiBRAIN@@QAA_NABV?$dsTSTRING@D@@00_N0@Z) — run "UseSmartObject" with
    // (object, action, anim, instant, exitAction); returns its bool result.
    bool UseSmartObject(const dsTSTRING<char> &object, const dsTSTRING<char> &action,
                        const dsTSTRING<char> &anim, bool instant, const dsTSTRING<char> &exitAction);
    // 0x8324EA60 (?UseSmartObjectInstant@aiBRAIN@@QAA_NABV?$dsTSTRING@D@@0_N@Z) — run
    // "UseSmartObjectInstant" with (object, action, keepPlaying); returns its bool result.
    bool UseSmartObjectInstant(const dsTSTRING<char> &object, const dsTSTRING<char> &action, bool keepPlaying);
    // 0x8324ECB8 (?AttachBodyToSmtObj@aiBRAIN@@QAA_NABV?$dsTSTRING@D@@0@Z) — run "AttachBodyToSmtObj"
    // with (object, anchor); returns its bool result.
    bool AttachBodyToSmtObj(const dsTSTRING<char> &object, const dsTSTRING<char> &anchor);
    // 0x832677D0 (?CheckCallForceAF_Smt@aiBRAIN@@...) — SSL guard for a "ForceAF smart" call: unbox
    // the smart-object name from funcParams[0], enumerate its entry-point slots and test whether the
    // body can use any. Returns true unless the object exists, has slots and none are usable.
    // REVERSED: src/ws/ai/aiBRAIN__CheckCallForceAF_Smt.cpp.
    bool CheckCallForceAF_Smt(const dsVECTOR<struct dsDATA, 8> &funcParams);
    // 0x832698E8 (?TryToUseAnimDeathSmt@aiBRAIN@@...) — find the nearest usable registered anim-death
    // smart object within the status max distance and push an "AnimDeathSmt" SMART behaviour toward
    // it (flags the death state + re-activates the body). REVERSED: src/ws/ai/aiBRAIN__TryToUseAnimDeathSmt.cpp.
    bool TryToUseAnimDeathSmt();
    // 0x8324EEC8 (?UseTurret@aiBRAIN@@QAA_NABV?$dsTSTRING@D@@0_N1@Z) — run "UseTurret" with
    // (turret, action, instant, keep); returns its bool result.
    bool UseTurret(const dsTSTRING<char> &turret, const dsTSTRING<char> &action, bool instant, bool keep);
    // 0x8324F168 (?cbFindBestTurret@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback:
    // read (float, bool) from `argv`, forward to implFindBestTurret, store the turret name into `retVal`.
    void cbFindBestTurret(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x8324F268 (?UseSmartIdleInstant@aiBRAIN@@QAA_NABV?$dsTSTRING@D@@0@Z) — run "UseSmartIdleInstant"
    // with (object, action); returns its bool result.
    bool UseSmartIdleInstant(const dsTSTRING<char> &object, const dsTSTRING<char> &action);
    // 0x8324F478 (?GetFloat@aiBRAIN@@QAAMABV?$dsTSTRING@D@@@Z) — run "GetFloat" with a param name;
    // returns the float result.
    float GetFloat(const dsTSTRING<char> &name);
    // 0x8324F5F8 (?SetFloat@aiBRAIN@@QAAXABV?$dsTSTRING@D@@M@Z) — run "SetFloat" with (name, value).
    void SetFloat(const dsTSTRING<char> &name, float value);

    // --- impl callees dispatched by the callbacks above (next frontier — bodies external). boundary
    // 0x832... (?implFindExPartner@aiBRAIN@@...) — the exclamation partner actor within `range`.
    iaIACTOR *implFindExPartner(float range);
    // 0x832... (?implHasHOInNavSys@aiBRAIN@@...) — whether a hold-object exists in this nav-system.
    bool implHasHOInNavSys(int argc, dsDATA *argv);
    // 0x832... (?implGetDistToObj@aiBRAIN@@...) — the float distance from this brain to `obj`.
    float implGetDistToObj(iaIACTOR *obj);
    // 0x832AF2D0 (?implGetPathRemain@aiBRAIN@@UAAMXZ) — remaining length of the active PATH behaviour
    // (or -1 when none). REVERSED: aiBRAIN__implGetPathRemain.cpp.
    float implGetPathRemain();
    // 0x832AF338 (?implGetDistPlayerToPathEnd@aiBRAIN@@UAAMXZ) — distance from the tracked player to
    // the active PATH behaviour's end (or -1 when none). REVERSED: aiBRAIN__implGetDistPlayerToPathEnd.cpp.
    float implGetDistPlayerToPathEnd();
    // 0x83285910 (?implFindBestTurret@aiBRAIN@@...) — the name of the best turret within `radius`
    // (returned by value; dispatched by cbFindBestTurret). Iterates all msg-system entities,
    // collects turrets that CanAimAtPoint (optionally angle-checked), returns the nearest one's name.
    // REVERSED: src/ws/ai/aiBRAIN__implFindBestTurret.cpp.
    dsTSTRING<char> implFindBestTurret(float radius, bool isCheckAngles);
    // 0x83286000 (?ForceAFUseTurret@aiBRAIN@@...) — teleport ("ForceAF") the brain onto the best
    // available entry point of `smtTurret` and set/push a turret behaviour named `bhvName`_TUR.
    // REVERSED: src/ws/ai/aiBRAIN__ForceAFUseTurret.cpp.
    bool ForceAFUseTurret(dsSHARED_PTR<struct smtSMART_OBJ_BASE, 0, Deleter<struct smtSMART_OBJ_BASE> > smtTurret,
                          const dsTSTRING<char> &bhvName, bool checkAngles, bool doPushBHV);

    // --- turret-mount helpers (bodies external to this batch). boundary ---
    // 0x832AE950 (?Transport@aiBRAIN@@UAAXABUm3dV@@@Z) — instantly move the brain's body to world
    // position `pos`. REVERSED: aiBRAIN__Transport_pos.cpp.
    void Transport(const m3dV *pos);
    // 0x832AED00 (?Transport@aiBRAIN@@UAAXAAVfsmCB_POS_DIR_PAR@@@Z) — instantly move the brain's body
    // to the pos/dir/up described by `par` (fires the body FSM "reset" callback with it). REVERSED:
    // aiBRAIN__Transport_par.cpp.
    void Transport(struct fsmCB_POS_DIR_PAR &par);
    // 0x832... (?OnBhvEnd@aiBRAIN@@...) — notify the brain the named behaviour ended.
    void OnBhvEnd(const char *bhvName);
    // 0x832... (?OnBhvEndSuccess@aiBRAIN@@...) — notify the brain the named behaviour ended successfully.
    void OnBhvEndSuccess(const char *bhvName);
    // 0x832... (?OnBhvEndFailed@aiBRAIN@@...) — notify the brain the named behaviour ended in failure.
    void OnBhvEndFailed(const char *bhvName);

    // 0x832AFCD8 (?ForceAFMoveToPoint@aiBRAIN@@MAA_NABV?$dsTSTRING@D@@0_N@Z) — protected: "auto-finish"
    // MoveToPoint — teleport straight to the compound-path destination waypoint (adopting its nav
    // system) instead of walking the path, firing the behaviour-end events. REVERSED:
    // aiBRAIN__ForceAFMoveToPoint.cpp.
    bool ForceAFMoveToPoint(const dsTSTRING<char> &wpName, const dsTSTRING<char> &bhvName, bool doPushBHV);

    // 0x832B0B48 (?implTransport@aiBRAIN@@UAAXABV?$dsTSTRING@D@@_N@Z) — virtual: teleport the brain's
    // body to the waypoint/tracked object named `nameTrk` (using its orientation), then finish all
    // path/HO/smart/turret behaviours. REVERSED: src/ws/ai/aiBRAIN__implTransport.cpp.
    void implTransport(const dsTSTRING<char> &nameTrk, bool isUseOrient);
    // 0x832B0498 (?ForceAFMoveToNavSys@aiBRAIN@@MAA_NABV?$dsTSTRING@D@@0_N@Z) — protected virtual:
    // "force after-frame" scripted move onto nav system `nsName`: snap to its compound-path
    // destination waypoint, localize, and notify behaviour `bhvName` succeeded. REVERSED:
    // src/ws/ai/aiBRAIN__ForceAFMoveToNavSys.cpp.
    bool ForceAFMoveToNavSys(const dsTSTRING<char> &nsName, const dsTSTRING<char> &bhvName, bool doPushBHV);
    // 0x832B1510 (?ForceAFMoveToHO@aiBRAIN@@MAA_NABV?$dsTSTRING@D@@0_N@Z) — protected virtual: "force
    // after-frame" scripted move onto hideout `hoName`: snap to the HO destination waypoint, acquire it,
    // and set/push an HO behaviour. REVERSED: src/ws/ai/aiBRAIN__ForceAFMoveToHO.cpp.
    bool ForceAFMoveToHO(const dsTSTRING<char> &hoName, const dsTSTRING<char> &bhvName, bool doPushBHV);

    // --- SSL nav/smart-object/turret/float callbacks + string-returning dispatch (0x83257B38-0x83258B4C).
    //     Each cb* is the native SSL callback that unboxes argv and forwards to the matching virtual
    //     impl*; GetClosestPoint/FindBestTurret run their per-species SSL func through sslObject.CallFunc
    //     (spDesc.pointee — see aiBRAIN_DESC_sslfunc_boundary.h). REVERSED: src/ws/ai/aiBRAIN__<Method>.cpp ---
    // 0x83257B38 (?cbMoveToHO@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — read (ho, arrive, style,
    // run) from `argv`, forward to implMoveToHO, store the bool into `retVal`.
    void cbMoveToHO(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83257C80 (?cbMoveToNavSys@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — read (navSys, arrive,
    // style, run) from `argv`, forward to implMoveToNavSys, store the bool into `retVal`.
    void cbMoveToNavSys(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83257DC8 (?cbGetDistToWP@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — read a waypoint name
    // from `argv`, forward to implGetDistToWP, store the float into `retVal`.
    void cbGetDistToWP(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83257E80 (?GetClosestPoint@aiBRAIN@@QAA?AV?$dsTSTRING@D@@ABV2@0@Z) — run "GetClosestPoint" with
    // (points, filter); returns the chosen point name by value.
    dsTSTRING<char> GetClosestPoint(const dsTSTRING<char> &points, const dsTSTRING<char> &filter);
    // 0x832580C8 (?cbGetClosestPoint@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — read (points,
    // filter) from `argv`, forward to implGetClosestPoint, store the returned name into `retVal`.
    void cbGetClosestPoint(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83258218 (?cbSetForceSpeed@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — read a speed-preset
    // name from `argv`, forward to implSetForceSpeed, store the bool into `retVal`.
    void cbSetForceSpeed(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x832582D0 (?cbUseSmartObject@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — read (object,
    // action, anim, instant, exitAction) from `argv`, forward to implUseSmartObject, store the bool.
    void cbUseSmartObject(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83258458 (?cbUseSmartObjectInstant@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — read
    // (object, action, keepPlaying) from `argv`, forward to implUseSmartObjectInstant, store the bool.
    void cbUseSmartObjectInstant(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83258568 (?cbAttachBodyToSmtObj@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — read (object,
    // anchor) from `argv`, forward to implAttachBodyToSmtObj, store the bool into `retVal`.
    void cbAttachBodyToSmtObj(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83258660 (?cbUseTurret@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — read (turret, action,
    // instant, keep) from `argv`, forward to implUseTurret, store the bool into `retVal`.
    void cbUseTurret(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83258780 (?FindBestTurret@aiBRAIN@@QAA?AV?$dsTSTRING@D@@M_N@Z) — run "FindBestTurret" with
    // (range, flag); returns the best turret name by value.
    dsTSTRING<char> FindBestTurret(float range, bool flag);
    // 0x83258920 (?cbUseSmartIdleInstant@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — read (object,
    // action) from `argv`, forward to implUseSmartIdleInstant, store the bool into `retVal`.
    void cbUseSmartIdleInstant(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83258A18 (?cbGetFloat@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — read a param name from
    // `argv`, forward to implGetFloat, store the float into `retVal`.
    void cbGetFloat(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83258AD0 (?cbSetFloat@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — read (name, value) from
    // `argv`, forward to implSetFloat.
    void cbSetFloat(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);

    // --- impl callees dispatched by the callbacks above (virtuals; bodies external — next frontier). boundary
    // 0x832... (?implMoveToHO@aiBRAIN@@...) — move the brain to hold-object `ho`; returns success.
    bool implMoveToHO(const dsTSTRING<char> &ho, const dsTSTRING<char> &arrive,
                      const dsTSTRING<char> &style, bool run);
    // 0x832... (?implMoveToNavSys@aiBRAIN@@...) — move the brain along nav-system `navSys`.
    bool implMoveToNavSys(const dsTSTRING<char> &navSys, const dsTSTRING<char> &arrive,
                          const dsTSTRING<char> &style, bool run);
    // 0x832... (?implGetDistToWP@aiBRAIN@@...) — the float distance from this brain to waypoint `wp`.
    float implGetDistToWP(const dsTSTRING<char> &wp);
    // 0x832... (?implGetClosestPoint@aiBRAIN@@...) — closest point name among `points` matching `filter`.
    dsTSTRING<char> implGetClosestPoint(const dsTSTRING<char> &points, const dsTSTRING<char> &filter);
    // 0x832... (?implSetForceSpeed@aiBRAIN@@...) — force the brain's move speed to preset `speed`.
    bool implSetForceSpeed(const dsTSTRING<char> &speed);
    // 0x832... (?implUseSmartObject@aiBRAIN@@...) — play a smart-object interaction.
    bool implUseSmartObject(const dsTSTRING<char> &object, const dsTSTRING<char> &action,
                            const dsTSTRING<char> &anim, bool instant, const dsTSTRING<char> &exitAction);
    // 0x832... (?implUseSmartObjectInstant@aiBRAIN@@...) — snap into a smart-object interaction.
    bool implUseSmartObjectInstant(const dsTSTRING<char> &object, const dsTSTRING<char> &action, bool keepPlaying);
    // 0x832... (?implAttachBodyToSmtObj@aiBRAIN@@...) — attach the brain's body to a smart-object anchor.
    bool implAttachBodyToSmtObj(const dsTSTRING<char> &object, const dsTSTRING<char> &anchor);
    // 0x832... (?implUseTurret@aiBRAIN@@...) — mount/operate a turret smart-object.
    bool implUseTurret(const dsTSTRING<char> &turret, const dsTSTRING<char> &action, bool instant, bool keep);
    // 0x832... (?implUseSmartIdleInstant@aiBRAIN@@...) — snap into a smart-object idle animation.
    bool implUseSmartIdleInstant(const dsTSTRING<char> &object, const dsTSTRING<char> &action);

    // --- aiBRAIN param/timer/group SSL-dispatch batch (0x8324F798-0x832505E8). Each reads a
    //     per-species SSL id out of spDesc.pointee (aiBRAIN_DESC_sslfunc_boundary.h) and dispatches
    //     it through sslObject.CallFunc.  REVERSED: src/ws/ai/aiBRAIN__<Method>.cpp ---
    // 0x8324F798 (?GetBool@aiBRAIN@@QAA_NABV?$dsTSTRING@D@@@Z) — run "GetBool" with a param name; bool.
    bool GetBool(const dsTSTRING<char> &name);
    // 0x8324F910 (?GetInt@aiBRAIN@@QAAHABV?$dsTSTRING@D@@@Z) — run "GetInt" with a param name; int.
    int GetInt(const dsTSTRING<char> &name);
    // 0x8324FA88 (?SetTimer@aiBRAIN@@QAAHM_N@Z) — run "SetTimer" with (duration, isCycle); int timer id.
    int SetTimer(float duration, bool isCycle);
    // 0x8324FBF0 (?cbSetTimer@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback: read
    // (float duration, bool isCycle) from `argv`, forward to implSetTimer, store the int id in `retVal`.
    void cbSetTimer(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x8324FCA0 (?KillTimer@aiBRAIN@@QAAXH@Z) — run "KillTimer" with a timer id.
    void KillTimer(int timerID);
    // 0x8324FD88 (?cbKillTimer@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback: read an
    // int timer id from `argv` and forward to implKillTimer.
    void cbKillTimer(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x8324FDF0 (?OnTimer@aiBRAIN@@QAAXH@Z) — run "OnTimer" with a timer id.
    void OnTimer(int timerID);
    // 0x8324FED8 (?cbOnTimer@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback: read an
    // int timer id from `argv` and forward to implOnTimer.
    void cbOnTimer(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x8324FF40 (?JoinGroup@aiBRAIN@@QAA_NABV?$dsTSTRING@D@@0@Z) — run "JoinGroup" with (group, tag); bool.
    bool JoinGroup(const dsTSTRING<char> &group, const dsTSTRING<char> &tag);
    // 0x83250150 (?LeaveGroup@aiBRAIN@@QAAXXZ) — run "LeaveGroup" (no args).
    void LeaveGroup();
    // 0x832501D0 (?HaveGroup@aiBRAIN@@QAA_NXZ) — run "HaveGroup" (no args); bool.
    bool HaveGroup();
    // 0x83250290 (?cbHaveGroup@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback: forward
    // (argc, argv) to implHaveGroup and store its bool result into `retVal`.
    void cbHaveGroup(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83250320 (?GetGroup@aiBRAIN@@QAA?AVsslOBJ_REF@@XZ) — run "GetGroup", resolve the returned
    // script object reference against the script system and return it (by value).
    sslOBJ_REF GetGroup();
    // 0x83250428 (?SetInterest@aiBRAIN@@QAAXABV?$dsTSTRING@D@@0_N@Z) — run "SetInterest" with
    // (subject, kind, enable).
    void SetInterest(const dsTSTRING<char> &subject, const dsTSTRING<char> &kind, bool enable);
    // --- impl callees of the timer/group batch above (next frontier — bodies external). boundary ---
    // 0x832... (?implOnTimer@aiBRAIN@@...) — handle timer `timerID` firing (dispatched by cbOnTimer).
    void implOnTimer(int timerID);
    // 0x832... (?implHaveGroup@aiBRAIN@@...) — whether the brain currently belongs to a group.
    bool implHaveGroup(int argc, dsDATA *argv);

    // --- SSL interest/team-name/neutral-count/danger/timer dispatch batch (0x83250648-0x83251350).
    //     Each public method reads a per-species SSL id out of spDesc.pointee
    //     (aiBRAIN_DESC_sslfunc_boundary.h) and dispatches via sslObject.CallFunc; the cb* methods are
    //     the matching SSL callbacks. REVERSED: src/ws/ai/aiBRAIN__<Method>.cpp ---
    // 0x83250648 (?EndInterest@aiBRAIN@@QAAXXZ) — run "EndInterest" (no args).
    void EndInterest();
    // 0x832506C8 (?Alert@aiBRAIN@@QAAXXZ) — run "Alert" (no args).
    void Alert();
    // 0x83250748 (?cbGetTeamName@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback: store
    // implGetTeamName's string into `retVal`.
    void cbGetTeamName(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83250818 (?SetTeamName@aiBRAIN@@QAAXABV?$dsTSTRING@D@@@Z) — run "SetTeamName" with a team name.
    void SetTeamName(const dsTSTRING<char> &teamName);
    // 0x83250958 (?CountMyNeutralsAroundMe@aiBRAIN@@QAAHM@Z) — run "CountMyNeutralsAroundMe" with a
    // radius; returns the int count.
    int CountMyNeutralsAroundMe(float radius);
    // 0x83250A80 (?cbCountMyNeutralsAroundMe@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL
    // callback: read a float from `argv`, forward to implCountMyNeutralsAroundMe, store int in `retVal`.
    void cbCountMyNeutralsAroundMe(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83250B18 (?CountMyNeutralsAroundEnemy@aiBRAIN@@QAAHM@Z) — run "CountMyNeutralsAroundEnemy"
    // with a radius; returns the int count.
    int CountMyNeutralsAroundEnemy(float radius);
    // 0x83250C40 (?cbCountMyNeutralsAroundEnemy@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL
    // callback: read a float from `argv`, forward to implCountMyNeutralsAroundEnemy, store int in `retVal`.
    void cbCountMyNeutralsAroundEnemy(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83250CD8 (?AvoidDanger@aiBRAIN@@QAA_NABV?$dsTSTRING@D@@@Z) — run "AvoidDanger" with a danger
    // name; returns its bool result.
    bool AvoidDanger(const dsTSTRING<char> &danger);
    // 0x83250E50 (?AvoidGrenade@aiBRAIN@@QAA_NHABV?$dsTSTRING@D@@M@Z) — run "AvoidGrenade" with
    // (grenadeId, danger, radius); returns its bool result.
    bool AvoidGrenade(int grenadeId, const dsTSTRING<char> &danger, float radius);
    // 0x83251060 (?tClear@aiBRAIN@@QAAXXZ) — run "tClear" (no args).
    void tClear();
    // 0x832510E0 (?tWaitBHV@aiBRAIN@@QAAXABV?$dsTSTRING@D@@@Z) — run "tWaitBHV" with a behaviour name.
    void tWaitBHV(const dsTSTRING<char> &bhv);
    // 0x83251220 (?tLock@aiBRAIN@@QAAXH@Z) — run "tLock" with an int argument.
    void tLock(int arg);
    // 0x83251308 (?cbtLock@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback: read an int
    // from `argv` and forward to impltLock.
    void cbtLock(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x8325FC90 (?impltLock@aiBRAIN@@UAAXH@Z) — virtual, next frontier (body external): the tLock impl.
    void impltLock(int arg);

    // --- SSL hideout / shoot / enemy-aim dispatch batch (0x83255950-0x83256AC0).
    //     REVERSED: src/ws/ai/aiBRAIN__<Method>.cpp ---
    // OnEnterHO / OnLeaveHO (0x83255950 / 0x83255A30) are declared above (near the other hideout
    // hooks); their REVERSED bodies live in aiBRAIN__OnEnterHO.cpp / aiBRAIN__OnLeaveHO.cpp. The
    // duplicate declarations that once sat here were removed (C2535).

    // 0x8329AA28 (?OnApproach@aiBRAIN@@QAAXAAV?$dsTSTRING@D@@_N@Z) — fire the descriptor OnApproach
    // enter/leave SSL event on this brain, passing the approach `name` as the event payload.
    // REVERSED: aiBRAIN__OnApproach.cpp.
    void OnApproach(dsTSTRING<char> &name, bool isEnter);
    // (implSetApproach is already declared above near the impl* SSL block.)
    // 0x83255B10 (?implGetBool@aiBRAIN@@UAA_NABV?$dsTSTRING@D@@@Z) — virtual SSL op: read the named
    //   bool SSL parameter `desc` (false + log when unknown). REVERSED: aiBRAIN__implGetBool.cpp.
    bool implGetBool(const dsTSTRING<char> &desc);
    // 0x832561C0 (?implPushSHOOT@aiBRAIN@@UAA_NABV?$dsTSTRING@D@@@Z) — virtual SSL op: push a SHOOT
    //   behaviour named `bhvName` onto the best-behaviour stack (false + log on failure/back-off).
    //   REVERSED: aiBRAIN__implPushSHOOT.cpp.
    bool implPushSHOOT(const dsTSTRING<char> &bhvName);
    // 0x832562D8 (?implGetString@aiBRAIN@@UAA?AV?$dsTSTRING@D@@ABV2@@Z) — virtual SSL op: read the
    //   named string SSL parameter `desc` (empty + log when unknown). REVERSED: aiBRAIN__implGetString.cpp.
    dsTSTRING<char> implGetString(const dsTSTRING<char> &desc);
    // 0x83256680 (?cbSetEnemy@...) — SSL callback: unbox argv[0] string, forward to implSetEnemy.
    void cbSetEnemy(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83256708 (?cbIsEnemyType@...) — SSL callback: unbox argv[0] string, store implIsEnemyType bool.
    void cbIsEnemyType(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x832567C0 (?cbSetAim@...) — SSL callback: unbox argv[0] string, forward to implSetAim.
    void cbSetAim(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83256848 (?GetAim@aiBRAIN@@QAA?AV?$dsTSTRING@D@@XZ) — run the descriptor "GetAim" SSL function
    //   and return its string result by value. REVERSED: aiBRAIN__GetAim.cpp.
    dsTSTRING<char> GetAim();
    // 0x83256928 (?cbSetPar@...) — SSL callback: read float argv[1] + string argv[0], forward to implSetPar.
    void cbSetPar(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x832569B0 (?cbSetSenses@...) — SSL callback: unbox argv[0] string, forward to implSetSenses.
    void cbSetSenses(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83256A38 (?cbShootNow@...) — SSL callback: unbox argv[0] string, forward to implShootNow.
    void cbShootNow(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83256AC0 (?cbEnableStrike@...) — SSL callback: unbox argv[0] string, forward to implEnableStrike.
    void cbEnableStrike(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83256B48 (?cbDisableStrike@...) — SSL callback: unbox argv[0] string, forward to implDisableStrike.
    void cbDisableStrike(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83256BD0 (?cbLockStrike@...) — SSL callback: unbox argv[0] string + argv[1] float, forward to implLockStrike.
    void cbLockStrike(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83256C58 (?cbDbgStrike@...) — SSL callback: unbox argv[0] string, forward to implDbgStrike.
    void cbDbgStrike(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83256CE0 (?cbSetWpn@...) — SSL callback: unbox argv[0] string, forward to implSetWpn.
    void cbSetWpn(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83256D68 (?cbSetDamageCurve@...) — SSL callback: unbox argv[0] string, forward to implSetDamageCurve.
    void cbSetDamageCurve(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83256ED0 (?cbSetBHV@...) — SSL callback: unbox argv[0] string, forward to implSetBHV.
    void cbSetBHV(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83256F58 (?cbPushBHV@...) — SSL callback: unbox argv[0] string, forward to implPushBHV.
    void cbPushBHV(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83256FE0 (?cbPushSHOOT@...) — SSL callback: unbox argv[0] string, forward to implPushSHOOT, return bool.
    void cbPushSHOOT(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);

    // Virtual SSL impls forwarded to by the cb* callbacks above. Bodies are the next frontier
    // (body/mind external to this batch). boundary.
    void implSetEnemy(const dsTSTRING<char> &name);      // 0x8325xxxx ?implSetEnemy@aiBRAIN@@UAAX...
    bool implIsEnemyType(const dsTSTRING<char> &name);   // ?implIsEnemyType@aiBRAIN@@UAA_N...
    void implSetAim(const dsTSTRING<char> &name);        // ?implSetAim@aiBRAIN@@UAAX...
    void implEndAim();                                   // 0x8329A0E0 ?implEndAim@aiBRAIN@@UAAXXZ. REVERSED: aiBRAIN__implEndAim.cpp
    void implEndInterest();                              // 0x8329A550 ?implEndInterest@aiBRAIN@@UAAXXZ. REVERSED: aiBRAIN__implEndInterest.cpp
    // implSetSenses declared above (canonical, ~line 114) — duplicate removed.
    void implShootNow(const dsTSTRING<char> &name);      // ?implShootNow@aiBRAIN@@UAAX...
    void implEnableStrike(const dsTSTRING<char> &name);  // ?implEnableStrike@aiBRAIN@@UAAX...
    void implDisableStrike(const dsTSTRING<char> &name); // ?implDisableStrike@aiBRAIN@@UAAX...
    void implLockStrike(const dsTSTRING<char> &name, float lockTime); // ?implLockStrike@aiBRAIN@@UAAX...
    void implDbgStrike(const dsTSTRING<char> &name);     // ?implDbgStrike@aiBRAIN@@UAAX...
    void implSetWpn(const dsTSTRING<char> &name);        // ?implSetWpn@aiBRAIN@@UAAX...
    void implSetDamageCurve(const dsTSTRING<char> &name);// ?implSetDamageCurve@aiBRAIN@@UAAX...
    void implSetBHV(const dsTSTRING<char> &name);        // ?implSetBHV@aiBRAIN@@UAAX...
    void implPushBHV(const dsTSTRING<char> &name);       // ?implPushBHV@aiBRAIN@@UAAX...

    // --- SSL get/set-param + team/interest/danger/join-group callback batch (0x83258B58-0x83259460).
    //     Each cb* is the SSL-registered callback: unbox its dsDATA argv, forward to the matching
    //     virtual impl*, box any result into retVal. The Get* methods dispatch a per-species SSL
    //     function id (aiBRAIN_DESC_sslfunc_boundary.h) via sslObject.CallFunc.
    //     REVERSED: src/ws/ai/aiBRAIN__<Method>.cpp ---
    // 0x83258B58 (?cbGetBool@...) — read string argv[0], store implGetBool's bool into retVal.
    void cbGetBool(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83258C10 (?cbGetInt@...) — read string argv[0], store implGetInt's int into retVal.
    void cbGetInt(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83258CC8 (?GetString@aiBRAIN@@QAA?AV?$dsTSTRING@D@@ABV2@@Z) — run the descriptor "GetString"
    //   SSL function with a key string argument; return its string result by value.
    dsTSTRING<char> GetString(const dsTSTRING<char> &key);
    // 0x83258E60 (?cbGetString@...) — read string argv[0], store implGetString's string into retVal.
    void cbGetString(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83258F70 (?cbJoinGroup@...) — read strings argv[0],argv[1], store implJoinGroup's bool into retVal.
    void cbJoinGroup(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83259068 (?cbSetInterest@...) — read strings argv[0],argv[1] + bool argv[2], forward to implSetInterest.
    void cbSetInterest(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83259130 (?GetTeamName@aiBRAIN@@QAA?AV?$dsTSTRING@D@@XZ) — run the descriptor "GetTeamName"
    //   SSL function (no args) and return its string result by value.
    dsTSTRING<char> GetTeamName();
    // 0x83259210 (?cbSetTeamName@...) — read string argv[0], forward to implSetTeamName.
    void cbSetTeamName(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83259298 (?cbAvoidDanger@...) — read string argv[0], store implAvoidDanger's bool into retVal.
    void cbAvoidDanger(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83259350 (?cbAvoidGrenade@...) — read int argv[0] + string argv[1] + float argv[2], store
    //   implAvoidGrenade's bool into retVal.
    void cbAvoidGrenade(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83259438 (?cbtWaitBHV@...) — read string argv[0], forward to impltWaitBHV.
    void cbtWaitBHV(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x832594C0 (?QOT_Put@aiBRAIN@@QAAXV?$REF_TYPE@VdsVECTOR_PARAM_LIST@@@ds_data@@@Z) — run the
    //   descriptor "QOT_Put" SSL function with a param-list reference argument (no return value).
    void QOT_Put(ds_data::REF_TYPE<dsVECTOR_PARAM_LIST> params);
    // 0x8325B6F8 (?TestParam2AI@aiBRAIN@@QAAXV?$REF_TYPE@VdsVECTOR_PARAM_LIST@@@ds_data@@@Z) — run the
    //   descriptor "TestParam2AI" SSL function with a param-list reference argument (no return value).
    //   REVERSED: src/ws/ai/aiBRAIN__TestParam2AI.cpp.
    void TestParam2AI(ds_data::REF_TYPE<dsVECTOR_PARAM_LIST> params);
    // 0x8325B808 (?implTestParam2AI@aiBRAIN@@UAAXV?$REF_TYPE@VdsVECTOR_PARAM_LIST@@@ds_data@@@Z, virtual)
    //   — native worker for the "TestParam2AI" SSL callback: push `params` onto the file-static AI-test
    //   task queue. REVERSED: src/ws/ai/aiBRAIN__implTestParam2AI.cpp.
    void implTestParam2AI(ds_data::REF_TYPE<dsVECTOR_PARAM_LIST> params);
    // 0x8325A8F8 (?implTerminate@aiBRAIN@@UAAXM@Z, virtual) — terminate the brain by issuing
    //   Die("REMOVE"); the float time argument is unused. REVERSED: src/ws/ai/aiBRAIN__implTerminate.cpp.
    void implTerminate(float time);

    // Virtual SSL impls forwarded to by the cb* callbacks above. Bodies are the next frontier. boundary.
    bool implJoinGroup(const dsTSTRING<char> &group, const dsTSTRING<char> &type);       // 0x832BE9B8 ?implJoinGroup@aiBRAIN@@UAA_N...
    void implSetInterest(const dsTSTRING<char> &name, const dsTSTRING<char> &kind, bool on); // 0x8329A3D0 ?implSetInterest@aiBRAIN@@UAAX...
    bool implAvoidDanger(const dsTSTRING<char> &danger);                                  // 0x83278B20 ?implAvoidDanger@aiBRAIN@@UAA_N...
    bool implAvoidGrenade(int grenadeId, const dsTSTRING<char> &danger, float radius);    // 0x83279598 ?implAvoidGrenade@aiBRAIN@@UAA_N...
    void impltWaitBHV(const dsTSTRING<char> &bhv);                                        // 0x8325FC28 ?impltWaitBHV@aiBRAIN@@UAAX...

    // --- SSL callbacks registered by aiBRAIN_DESC::InitSslClass (0x8325BCB8) whose bodies live
    //     outside this batch (next frontier), plus their impl callees. Uniform SSL cb ABI:
    //     void cbX(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller). boundary.
    void cbOnCreate(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);           // 0x832481E8
    void cbOnDestroy(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);          // 0x83248228
    void cbOnFrameBegin(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);       // 0x83248268
    void cbOnDbgOut(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);           // 0x832482A8
    void cbDropWeapon(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);         // 0x832482E8
    void cbEndEnemy(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);           // 0x83248328
    void cbEndEnemySoft(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);       // 0x83248368
    void cbEndAim(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);             // 0x832483A8
    void cbResetAlerts(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);        // 0x832483E8
    void cbForceWar(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);           // 0x83248428
    void cbForcePeace(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);         // 0x83248468
    void cbAutoBattle(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);         // 0x832484A8
    void cbCancelCurStrike(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);    // 0x832484E8
    void cbEnemyResetMovementDist(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller); // 0x83248528
    void cbThrowWeaponsAndItems(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);   // 0x83248568
    void cbEndApproach(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);        // 0x832485A8
    void cbForgetStuck(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);        // 0x832485E8
    void cbEndForceSpeed(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);      // 0x83248628
    void cbLeaveGroup(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);         // 0x83248668
    void cbEndInterest(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);        // 0x832486A8
    void cbAlert(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);              // 0x832486E8
    void cbtClear(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);             // 0x83248728
    void cbQOT_Reset(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);          // 0x83248768
    void cbQOT_ClearFront(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);     // 0x832487A8
    void cbtWait(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);              // 0x83260A10

    // 0x8325BB78 (?cbTestParam2AI@aiBRAIN@@QAA...) — SSL callback: retrieve argv[0] param-list ref,
    //   forward to implTestParam2AI. REVERSED: src/ws/ai/aiBRAIN__cbTestParam2AI.cpp.
    void cbTestParam2AI(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x8325BC18 (?cbQOT_Put@aiBRAIN@@QAA...) — SSL callback: retrieve argv[0] param-list ref,
    //   forward to implQOT_Put. REVERSED: src/ws/ai/aiBRAIN__cbQOT_Put.cpp.
    void cbQOT_Put(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);

    // 0x8325FBD8 (?impltClear@aiBRAIN@@UAAXXZ) — virtual: log + clear the task mind module.
    //   REVERSED: src/ws/ai/aiBRAIN__impltClear.cpp.
    void impltClear();
    // 0x8325B930 (?implThrowWeaponsAndItems@aiBRAIN@@UAAXXZ) — virtual: mark dying, then throw all
    //   weapons/items held by the prop container. REVERSED: src/ws/ai/aiBRAIN__implThrowWeaponsAndItems.cpp.
    void implThrowWeaponsAndItems();
    // impl callee of cbQOT_Put (virtual; body external — next frontier). boundary.
    void implQOT_Put(ds_data::REF_TYPE<dsVECTOR_PARAM_LIST> params);                      // ?implQOT_Put@aiBRAIN@@UAAX...

    // 0x8325F7A0 (?CheckCallForceAF_Default@aiBRAIN@@QBA_NABV?$dsVECTOR@VdsDATA@@$07@@@Z) — const:
    // default force-AF result predicate used by aiTASK_DATA_FN when a task-fn has no specific checker.
    // boundary (body is the next frontier).
    bool CheckCallForceAF_Default(const dsVECTOR<dsDATA, 8> &results) const;
};
