#pragma once
#include "../ds/dsSHARED_PTR.h"
#include "../ds/dsTSTRING.h"
#include "aiISA_BASE.h"
#include "AI_ALERT.h"
#include "aiSCRIPT_READER.h"
#include "aiTASKS_QUEUE.h"
// ws-engine ai08: the "mind" — the bundle of shared-pointer mind modules an aiBRAIN owns.
// DB-verified layout (types_members aiMIND) — size 0x9C (156). Only the `isa` module (dispatched
// by aiPLANNER::AlertTeam) is typed; the other modules are byte-accurate opaque placeholders so
// the offsets stay exact (their bodies are the next frontier).

struct aiBEST_BASE;     // aiWATCHER_BRAIN_boundaries.h — "best behaviour" module   boundary (fwd)
struct aiENEMY_BASE;    // aiWATCHER_BRAIN_boundaries.h — enemy-tracking module     boundary (fwd)
struct aiPERC_BASE;     // aiWATCHER_BRAIN_boundaries.h — perception module         boundary (fwd)
struct aiAIM_BASE;      // ai08 — aiming module        boundary (fwd)
struct aiAPPROACH_BASE; // ai08 — approach module      boundary (fwd)
struct aiGOAL_BASE;     // ai08 — goal module          boundary (fwd)
struct aiVISION_BASE;   // ai08 — vision module         boundary (fwd)
struct aiSELENE_BASE;   // ai08 — selene module        boundary (fwd)
struct aiSND_BASE;      // ai08 — sound module         boundary (fwd)
struct aiTASK_BASE;     // ai08 — task module          boundary (fwd)
struct aiMP_BASE;       // ai08 — mount/movement-prediction module  boundary (fwd)
// AI_ALERT — full enum in AI_ALERT.h (included above).
struct dmgDAMAGE;       // dmg subsystem — a damage event  boundary (fwd, used by NoticeWounded)

// aiMIND layout is DB-verified in full (types_members aiMIND) — every module is now typed at its
// exact offset, including scr (aiSCRIPT_READER) and qot (aiTASKS_QUEUE).
struct aiMIND {
    void         *__vftable; // 0x00 aiMIND_vtbl*
    void         *pBrain;    // 0x04 aiBRAIN*
    dsSHARED_PTR<aiAIM_BASE,      0, Deleter<aiAIM_BASE> >      aim;    // 0x08 aiming module
    dsSHARED_PTR<aiAPPROACH_BASE, 0, Deleter<aiAPPROACH_BASE> > aprog; // 0x10 approach module
    dsSHARED_PTR<aiBEST_BASE,     0, Deleter<aiBEST_BASE> >     best;   // 0x18 best-behaviour module
    dsSHARED_PTR<aiENEMY_BASE,    0, Deleter<aiENEMY_BASE> >    enemy;  // 0x20 enemy-tracking module
    dsSHARED_PTR<aiGOAL_BASE,     0, Deleter<aiGOAL_BASE> >     goal;   // 0x28 goal module
    dsSHARED_PTR<aiISA_BASE,      0, Deleter<aiISA_BASE> >      isa;    // 0x30 interop/social module
    dsSHARED_PTR<aiPERC_BASE,     0, Deleter<aiPERC_BASE> >     perc;   // 0x38 perception module
    dsSHARED_PTR<aiVISION_BASE,   0, Deleter<aiVISION_BASE> >   vision; // 0x40 vision module
    dsSHARED_PTR<aiSELENE_BASE,   0, Deleter<aiSELENE_BASE> >   selene; // 0x48 selene module
    aiSCRIPT_READER scr;     // 0x50 script-reader tunables block (DB-typed)
    dsSHARED_PTR<aiSND_BASE,      0, Deleter<aiSND_BASE> >      snd;    // 0x6C sound module
    dsSHARED_PTR<aiTASK_BASE,     0, Deleter<aiTASK_BASE> >     task;   // 0x74 task module
    aiTASKS_QUEUE qot;       // 0x7C queued-task storage (DB-typed)
    float         timerDelayedAlert; // 0x94
    AI_ALERT      alertDelayed;      // 0x98 pending delayed alert kind

    // 0x83227FB8 (?IsIdle@aiMIND@@QBA_NXZ) — true when the mind is in the idle state. boundary.
    bool IsIdle() const;

    // ai08 — does this mind accept the unit tracked by `cand` as an enemy? boundary.
    bool IsAcceptableEnemy(struct aiWATCHER *cand);

    // 0x83177704-region call — force-clear all mind modules (used during teardown). boundary.
    void ForcedClear();
    // 0x831789B4-region call — clear the mind's tracked enemy. boundary.
    void ZeroEnemy();
    // Per-level initial mind setup (called from aiBRAIN::ProcessINIT_LEVEL). boundary.
    void SetupInitial();

    // Per-init hooks driven by aiBRAIN::ProcessINIT. boundary (mind-module bodies are the next
    // frontier).
    bool IsLiveOnInit() const;   // true when the unit spawns alive
    void ProcessINITDead();      // init the mind modules for a dead spawn

    // 0x83229010 (?SetPar@aiMIND@@QAA_NABV?$dsTSTRING@D@@M@Z) — fan a named tunable out to the
    // status block + every mind module; true if any consumed it. REVERSED: aiMIND__SetPar.cpp.
    bool SetPar(const dsTSTRING<char> &key, float val);
    // 0x832290E8 (?CalcKeepDefCDTReqTime@aiMIND@@IBAMXZ) — protected: how long to keep default-CDT
    // requests alive, scaled by the count of aggressive squadmates. REVERSED: aiMIND__CalcKeepDefCDTReqTime.cpp.
    float CalcKeepDefCDTReqTime() const;
    // 0x832291E0 (?ProcessINITLive@aiMIND@@QAAXXZ) — init the mind modules for a live spawn. Reads
    // descriptor event/flag fields (aiBRAIN_DESC layout, next frontier). boundary — not yet reversed.
    void ProcessINITLive();
    // 0x83229348 (?CallSSL_OnComplete@aiMIND@@QAAXABV?$dsTSTRING@D@@@Z) — fire the SSL OnComplete /
    // OnDeath events (once) with the killing-damage name. Needs the aiBRAIN_DESC SSL-event-id
    // layout (next frontier). boundary — not yet reversed.
    void CallSSL_OnComplete(const dsTSTRING<char> &nameDamage);
    // 0x832294A8 (?CallSSL_OnBattleChanged@aiMIND@@QAAXXZ) — fire the SSL OnBattleChanged event.
    // Needs the aiBRAIN_DESC SSL-event-id layout (next frontier). boundary — not yet reversed.
    void CallSSL_OnBattleChanged();
    // 0x83229578 (?NoticeWounded@aiMIND@@QAAXABVdmgDAMAGE@@@Z) — propagate a wound event to SSL,
    // best/perception modules and the coordinator. Needs the aiBRAIN_DESC SSL-event-id layout
    // (next frontier). boundary — not yet reversed.
    void NoticeWounded(const dmgDAMAGE &dmg);
    // 0x83229658 (?UpdateSparser@aiMIND@@QAAXXZ) — update the mind-frame LOD/skip flag each tick.
    // REVERSED: aiMIND__UpdateSparser.cpp.
    void UpdateSparser();
    // 0x83229730 (?CanSetupInitial@aiMIND@@IAA_NXZ) — protected: whether initial setup may run now
    // (records the rejection reason for debug). REVERSED: aiMIND__CanSetupInitial.cpp.
    bool CanSetupInitial();
    // 0x832299B0 (?CallSSL_OnInit@aiMIND@@QAAXXZ) — fire the SSL OnInit event for a live brain.
    // Needs the aiBRAIN_DESC SSL-event-id layout (next frontier). boundary — not yet reversed.
    void CallSSL_OnInit();

    // 0x8322A980 / 0x8322AA30 — lazily-constructed default aim/approach modules (process-wide
    // singletons). REVERSED: aiMIND__GetDefSys_Aim.cpp / aiMIND__GetDefSys_Aprog.cpp.
    static dsSHARED_PTR<aiAIM_BASE,      0, Deleter<aiAIM_BASE> >      GetDefSys_Aim();
    static dsSHARED_PTR<aiAPPROACH_BASE, 0, Deleter<aiAPPROACH_BASE> > GetDefSys_Aprog();
    // 0x8322AAE0 (?GetDefSys_Best@aiMIND@@SA?AV?$dsSHARED_PTR@VaiBEST_BASE@@$0A@V?$Deleter@VaiBEST_BASE@@@@@@XZ)
    // — the process-wide default best-behaviour module (Meyers singleton, bare aiBEST_BASE).
    // REVERSED: aiMIND__GetDefSys_Best.cpp.
    static dsSHARED_PTR<aiBEST_BASE,     0, Deleter<aiBEST_BASE> >     GetDefSys_Best();
    // 0x8322AB90..0x8322AFA0 — the remaining lazily-constructed process-wide default modules.
    // REVERSED: aiMIND__GetDefSys_{Goal,Mp,Perc,Isa,Vision,Snd,Task}.cpp.
    static dsSHARED_PTR<aiGOAL_BASE,   0, Deleter<aiGOAL_BASE> >   GetDefSys_Goal();
    static dsSHARED_PTR<aiMP_BASE,     0, Deleter<aiMP_BASE> >     GetDefSys_Mp();
    static dsSHARED_PTR<aiPERC_BASE,   0, Deleter<aiPERC_BASE> >   GetDefSys_Perc();
    static dsSHARED_PTR<aiISA_BASE,    0, Deleter<aiISA_BASE> >    GetDefSys_Isa();
    static dsSHARED_PTR<aiVISION_BASE, 0, Deleter<aiVISION_BASE> > GetDefSys_Vision();
    static dsSHARED_PTR<aiSND_BASE,    0, Deleter<aiSND_BASE> >    GetDefSys_Snd();
    static dsSHARED_PTR<aiTASK_BASE,   0, Deleter<aiTASK_BASE> >   GetDefSys_Task();

    // 0x8322B1A8 (?DecideAppropriateVision@aiMIND@@IAAXXZ) — protected: swap the vision module
    // between idle/battle/cheap variants when perception flags a needed enhance/reduce.
    // REVERSED: aiMIND__DecideAppropriateVision.cpp.
    void DecideAppropriateVision();
    // 0x8322B4E0 (?FreezeEnemy@aiMIND@@QAAXXZ) — replace the tracked enemy with an aiENEMY_FROZEN.
    // REVERSED: aiMIND__FreezeEnemy.cpp.
    void FreezeEnemy();
    // 0x8322B600 (?ZeroISA@aiMIND@@QAAXXZ) — reset the isa/vision/aim modules to zeroed defaults.
    // REVERSED: aiMIND__ZeroISA.cpp.
    void ZeroISA();
    // 0x8322B798 (?ValidateEnemySystem@aiMIND@@IAAXXZ) — protected: freeze/zero the enemy module
    // depending on the tracked enemy's live/dead state. REVERSED: aiMIND__ValidateEnemySystem.cpp.
    void ValidateEnemySystem();

    // 0x83227D30 (?ForceDelayedCalls@aiMIND@@QAAXXZ) — if a delayed alert is pending, fire it on the
    // brain now and clear the timer. REVERSED: aiMIND__ForceDelayedCalls.cpp.
    void ForceDelayedCalls();
    // 0x83228038 (?UpdateTimers@aiMIND@@QAAXM@Z) — advance the delayed-alert timer and forward `time`
    // to every timed mind module. REVERSED: aiMIND__UpdateTimers.cpp.
    void UpdateTimers(float time);
    // 0x83228158 (?NoticeFlyObject@aiMIND@@QAAXABUm3dV@@0@Z) — a physics object flew past: record the
    // closest approach for debug and notify the perception + isa modules. REVERSED:
    // aiMIND__NoticeFlyObject.cpp.
    void NoticeFlyObject(const struct m3dV &posFrom, const struct m3dV &dirFly);
    // 0x832282C0 (?PostDelayedOnAlert@aiMIND@@QAAXW4AI_ALERT@@_N@Z) — queue `alert` to fire after a
    // (randomised, when isUseDelay) short delay. REVERSED: aiMIND__PostDelayedOnAlert.cpp.
    void PostDelayedOnAlert(AI_ALERT alert, bool isUseDelay);
    // 0x83228370 (?NoticeEnemyChanged@aiMIND@@QAAXPBVaiWATCHER@@PBVentENTITY@@@Z) — the tracked enemy
    // switched from `entOld` to the target of `wNew`: update cheap-vision, the player-attacker
    // registry and the weapon module. REVERSED: aiMIND__NoticeEnemyChanged.cpp.
    void NoticeEnemyChanged(const struct aiWATCHER *wNew, const struct entENTITY *entOld);

    // ---- saber2 ai-drain batch (a8_mind.cpp) --------------------------------------------------
    // 0x8322B868 (?InternalAssignEnemy@aiMIND@@IAA_NPAVentENTITY@@_NP6A_NPAVaiBRAIN@@QAVaiWATCHER@@@Z@Z)
    // — protected: try to make `ent` this mind's tracked enemy. Creates a watcher for `ent`, runs
    // `checker` to accept/reject it, then (on accept) installs a fresh aiENEMY module, re-starts the
    // aim module, updates the neutral-team status bit and the planner cheap-vision cache. Returns
    // true when the enemy was assigned. REVERSED: aiMIND__InternalAssignEnemy.cpp.
    bool InternalAssignEnemy(struct entENTITY *ent, int isVisible,
                             bool (*checker)(struct aiBRAIN *, struct aiWATCHER *));
    // 0x8322BFD0 (?AssignEnemy@aiMIND@@QAA_NPAVentENTITY@@_N@Z) — public: try to make `ent` this
    // mind's tracked enemy (delegates to the internal path with the default checker). Returns true
    // when assigned. Used by aiPERC::NoticeHeavyDamaged.  boundary (body external).
    bool AssignEnemy(struct entENTITY *ent, bool isVisible);
    // 0x8322BA98 (?ZeroSelene@aiMIND@@QAAXXZ) — swap the selene module for the null ("zero") arm
    // (an aiSELENE_IFACE re-vtabled to aiSELENE_ZERO). REVERSED: aiMIND__ZeroSelene.cpp.
    void ZeroSelene();
    // 0x8322BB68 (?SetNormalSelene@aiMIND@@QAAXXZ) — restore the concrete aiSELENE module (unless the
    // descriptor selects the null selene subsystem, in which case ZeroSelene). REVERSED:
    // aiMIND__SetNormalSelene.cpp.
    void SetNormalSelene();
    // 0x8322BCB0 (?ExecuteDelayedOnAlert@aiMIND@@IAAXXZ) — protected: if the delayed-alert timer has
    // just reached zero, fire the pending alert on the brain and reset the isa module. REVERSED:
    // aiMIND__ExecuteDelayedOnAlert.cpp.
    void ExecuteDelayedOnAlert();
    // 0x8322BD48 (?UpdateAllPerception@aiMIND@@IAAXM@Z) — protected: run one perception frame (isa,
    // perc, enemy visibility request, vision decision + processing). REVERSED:
    // aiMIND__UpdateAllPerception.cpp.
    void UpdateAllPerception(float dt);
    // 0x8322BE50 (?ProcessFrameAfterBody@aiMIND@@QAAXXZ) — post-body per-frame mind step: dead path,
    // or (alive) delayed-alert + best-behaviour after-body + task frame. REVERSED:
    // aiMIND__ProcessFrameAfterBody.cpp.
    void ProcessFrameAfterBody();
    // 0x8322BF60 (?StopScriptedEnemy@aiMIND@@QAAX_N@Z) — leave scripted-enemy mode: (optionally clear
    // the enemy), drop the scripted-enemy status bit, reset sparsings and restore normal selene.
    // REVERSED: aiMIND__StopScriptedEnemy.cpp.
    void StopScriptedEnemy(bool isClear);
    // 0x8322BFE0 (?SetupScriptedEnemy@aiMIND@@QAAXPAVentTRACKER@@@Z) — force the mind's enemy to the
    // entity referenced by tracker `tr` (or leave scripted mode when `tr` is null). REVERSED:
    // aiMIND__SetupScriptedEnemy.cpp.
    void SetupScriptedEnemy(struct entTRACKER *tr);
    // 0x8322C110 (?ValidateSeleneSystem@aiMIND@@IAAXXZ) — protected: if the enemy module went null
    // while scripted-enemy mode is still set, tear the scripted state down. REVERSED:
    // aiMIND__ValidateSeleneSystem.cpp.
    void ValidateSeleneSystem();
    // 0x8322C1A8 (?ProcessEnemySelection@aiMIND@@IAAXXZ) — protected: validate the enemy + selene
    // systems, then run one selene frame. REVERSED: aiMIND__ProcessEnemySelection.cpp.
    void ProcessEnemySelection();

    // 0x8322C4C0 (?EnsureDoneInitialSetup@aiMIND@@QAA_NXZ) — if the "needs initial setup" status bit
    // (gen bit 5) is still set, run SetupInitial; return true once initial setup is complete.
    // REVERSED: aiMIND__EnsureDoneInitialSetup.cpp.
    bool EnsureDoneInitialSetup();
    // 0x8322C528 (?ProcessFrameBeforeBody@aiMIND@@QAAXM@Z) — pre-body per-frame mind step: dead path,
    // fast (sparsed) path, or the full path (enemy selection, perception, best/approach/sound/aim
    // module frames). REVERSED: aiMIND__ProcessFrameBeforeBody.cpp.
    void ProcessFrameBeforeBody(float dt);

    // ---- referenced sibling mind methods (bodies external to this batch — next frontier) -------
    void ProcessFrameDeadA();       // dead-unit per-frame mind step                   boundary
    void ProcessFrameDeadB();       // 0x832AA6B8 dead-unit pre-body per-frame step     boundary
};
