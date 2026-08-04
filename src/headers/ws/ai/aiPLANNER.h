#pragma once
#include "../ds/dsVECTOR.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsSTRID.h"
#include "../ds/dsSTRID_FLAGS.h"
#include "../ds/dsFLAGS.h"
#include "../ds/dsSHARED_PTR.h"
#include "../m3d/m3dV.h"
#include "../m3d/m3dOBB.h"
#include "aiWAH_SYS.h"
#include "aiBASS.h"
#include "aiCHASER.h"
#include "aiCOORDINATOR.h"
#include "aiCHEAP_VISION.h"
#include "aiDEF_CDT_SYS.h"
#include "aiFILT_ENEMY.h"
#include "aiGLOSS.h"
#include "aiGRASS.h"
#include "aiLOD_SYS.h"
#include "aiPHYS_SMR.h"
#include "aiSTATISTICS.h"
#include "aiPLATT.h"
#include "../gs/gsTEAMS.h"
#include "../msg/msgRES.h" // msgRES — return type of ProcessMsg (below)

// ws-engine ai08 planner (D:\Projects\code\common\src.sys\gm_shared\ai08\a8_planner.cpp).
// DB-verified layout (types_members aiPLANNER) — total size 4816 (0x12D0). All subsystem blocks
// are typed per the DB member list (byteshim reconciliation 2026-08-04); only the aiPLANNER_IFACE
// base span stays opaque (vtable-based boundary, no standalone header).

enum AI_PLAST;  // ai08 — planner state flags (bit meanings are a boundary)
struct aiHO_SYS; // ai08 — HO query subsystem (aiHO_SYS.h); for ReconsiderHOChecker's member-fn ptr

struct aiSQUAD;           // aiSQUAD.h — squad object (registered/looked-up by the planner)
struct navMANAGER;        // nav subsystem — nav-mesh manager                    boundary (ptr)
struct navPENMAN;         // nav subsystem — penalty manager                     boundary (ptr)
struct nedNAV_EDITOR;     // ned subsystem — nav editor                          boundary (ptr)
struct bhvDSC_STORAGE;    // bhv subsystem — behavior descriptor storage         boundary (ptr)
struct aiTEAMS_SYS;       // ai08 — teams subsystem (size 4; opaque i_am_not_empty_class)
struct navSYS;            // nav subsystem — a single nav-system                 boundary
struct aiBRAIN;           // ai08 — per-unit AI brain                            boundary
struct smtSMART_OBJ_BASE; // smt subsystem — smart-object base                   boundary
struct entENTITY;         // entENTITY.h — game entity
struct propGAME_INFO_TEAM;// prop subsystem — team info                          boundary
struct smtSMT_ENTER_INFO; // smtSMT_ENTER_INFO.h — smart-object entry-point info
struct gsDOM_SPAWN;       // gs subsystem — spawn domain                         boundary (ptr)
struct aiNAV_WP_WRAPPER_TPL; // aiNAV_WP_WRAPPER_TPL.h
struct aiNAV_NS_WRAPPER_TPL; // aiNAV_NS_WRAPPER_TPL.h
struct dsDATA;            // ds/dsDATA.h — type-erased value (cb* SSL-callback params)
struct sslOBJ_REF;        // ssl/sslOBJ_REF.h — script object reference (cb*/emitter params)

struct aiPLANNER {
    unsigned char  iface[200];       // 0x0000 aiPLANNER_IFACE : iaIACTOR   boundary (vtable-based)
    aiBASS         bass;             // 0x00C8 exclamation/bark subsystem
    aiCHASER       chaser;           // 0x015C nav-system chase registry
    aiCOORDINATOR  coordinator;      // 0x0184 group coordinator subsystem
    aiCHEAP_VISION CheaVis;          // 0x0E44 cheap-vision cache
    aiDEF_CDT_SYS  defCDT;           // 0x0E5C default-CDT request system
    aiFILT_ENEMY   fe;               // 0x10EC enemy filter subsystem
    aiGLOSS        gloss;            // 0x1100 gloss subsystem
    aiGRASS        grass;            // 0x1128 grenade/rocket awareness subsystem
    aiLOD_SYS      LOD;              // 0x1168 LOD subsystem
    aiPHYS_SMR     smr;              // 0x11A0 SMR physics-feature subsystem
    int           teams;             // 0x11AC aiTEAMS_SYS (opaque i_am_not_empty_class)
    aiSTATISTICS   stat;             // 0x11B0 runtime statistics block
    aiWAH_SYS     wah;               // 0x1204 watcher subsystem (arrUnits/arrPlayers touched)
    aiPLATT       platt;             // 0x1254 platform-attack subsystem
    dsSTRID_FLAGS taskSysFlags;      // 0x126C active task-system flag names
    dsFLAGS<AI_PLAST, int> state;    // 0x1280 planner state flags
    navMANAGER   *navManager;        // 0x1284
    navPENMAN    *penManager;        // 0x1288
    nedNAV_EDITOR *navEditor;        // 0x128C
    bhvDSC_STORAGE *bhvDStore;       // 0x1290
    void         *bhvPSStore;        // 0x1294 psSECTION (opaque 4B in DB)
    unsigned int  bhvNextSNum;       // 0x1298
    dsVECTOR<aiSQUAD *, 8> arrSquad; // 0x129C registered squads
    float         hoMaxAngleCorner;  // 0x12B0
    float         hoMaxAngleMiddle;  // 0x12B4
    dsVECTOR<dsSHARED_PTR<smtSMART_OBJ_BASE, 0, Deleter<smtSMART_OBJ_BASE> >, 8> animDeathSmtObjects; // 0x12B8
    int           theMainAnswer;     // 0x12CC

    // --- methods reversed in this batch (a8_planner.cpp) ---
    void NoticeFlyObject(aiBRAIN *shooter, entENTITY *obj, const m3dV &from,
                         const m3dV &dir, const m3dOBB &obb, float speed);
    bool CanSetupInitial(aiBRAIN *pBrain, dsTSTRING<char> *rejectReason);
    bool DbgReasonWaitSetupInitial(dsTSTRING<char> &reason);
    void DbgTaskFlagsLine(dsTSTRING<char> &line);
    // 0x83197178 — virtual: hand every waypoint id in `wps` to the nav editor's "unused" set.
    void NoticeNavEdUnusedWP(const dsVECTOR<short, 8> &wps);
    // 0x831998D0 — protected: per-frame update of the debug trajectory-checker for the watched brain.
    void DbgTrajCheckProccessFrame();
    void UnRegisterSquad(aiSQUAD *pSquad);
    // 0x83177714-region call — remove `pBrain` from the planner's live-unit registry (called during
    // brain teardown). Body external to this batch. boundary.
    void UnRegisterUnit(aiBRAIN *pBrain);
    // Register `pBrain` in the planner's live-unit registry (called from aiBRAIN::ProcessINIT_LEVEL).
    // Body external to this batch. boundary.
    void RegisterUnit(aiBRAIN *pBrain);
    aiSQUAD *GetSquadByName(const dsTSTRING<char> &squadName);
    int  SelectNavSysIdx(dsVECTOR<navSYS *, 8> &acceptedNavSysList,
                         const m3dV &pos, const char *bname) const;
    void RegisterSquad(aiSQUAD *pSquad);
    void LocalizeToNavSystem(aiBRAIN *pBrain);
    void SetNavSystemToUnit(aiBRAIN *pBrain, const dsTSTRING<char> &nsName);
    dsSHARED_PTR<smtSMART_OBJ_BASE, 0, Deleter<smtSMART_OBJ_BASE> >
        FindSmartObject(const dsTSTRING<char> &nameInstSmt);
    bool InviteToSmartObject(const entENTITY *caller,
                             dsSHARED_PTR<smtSMART_OBJ_BASE, 0, Deleter<smtSMART_OBJ_BASE> > &smtObj,
                             const dsVECTOR<smtSMT_ENTER_INFO, 8> &entryPoints,
                             const propGAME_INFO_TEAM &team, bool canInviteAI, bool canInvitePlayer);

    // --- methods reversed in THIS batch (a8_planner.cpp) ---
    // 0x83163C30 (protected) — true when brain `br`'s world position lies inside `box`.
    // REVERSED: src/ws/ai/aiPLANNER__IsWithinBox.cpp.
    bool IsWithinBox(const m3dOBB *box, aiBRAIN *br);
    // 0x83163B48 (const, virtual) — allocate+construct a nav waypoint wrapper for `ent`.
    // REVERSED: src/ws/ai/aiPLANNER__CreateNavWPWrapperTpl.cpp.
    aiNAV_WP_WRAPPER_TPL *CreateNavWPWrapperTpl(entENTITY *ent) const;
    // 0x83163BB0 (const, virtual) — allocate+construct a nav-system wrapper for `ent`.
    // REVERSED: src/ws/ai/aiPLANNER__CreateNavNSWrapperTpl.cpp.
    aiNAV_NS_WRAPPER_TPL *CreateNavNSWrapperTpl(entENTITY *ent) const;
    // 0x831643C0 (const) — count live (non-dead) units spawned from `dom`.
    // REVERSED: src/ws/ai/aiPLANNER__CountLiveSpawnedFrom.cpp.
    int CountLiveSpawnedFrom(gsDOM_SPAWN *dom) const;
    // 0x83164450 (virtual) — notify all watchers/grass/pen-manager a grenade terminated.
    // REVERSED: src/ws/ai/aiPLANNER__NoticeGrenadeTerm.cpp.
    void NoticeGrenadeTerm(entENTITY *shooter, entENTITY *grenade);
    // 0x831644C0 (const) — count active units+players hostile to `teamTo`.
    // REVERSED: src/ws/ai/aiPLANNER__GetCountEnemiesTo.cpp.
    int GetCountEnemiesTo(const propGAME_INFO_TEAM &teamTo) const;
    // 0x831649B8 (virtual) — register a newly-thrown grenade with the pen-manager + grass.
    // REVERSED: src/ws/ai/aiPLANNER__NoticeGrenadeInit.cpp.
    void NoticeGrenadeInit(entENTITY *shooter, entENTITY *grenade, const m3dV *posEnd);
    // 0x83185278 (?implNoticeGrenadeExplosion@aiPLANNER@@UAAXPAViaIACTOR@@@Z, virtual) — a tracked
    // grenade `actor` detonated: tell every active watcher-brain, drop it from grass, and remove it
    // from the nav penalty manager. REVERSED: src/ws/ai/aiPLANNER__implNoticeGrenadeExplosion.cpp.
    virtual void implNoticeGrenadeExplosion(struct iaIACTOR *actor);
    // 0x83183870 (?implNoticeFlyingRocket@aiPLANNER@@UAAXPAViaIACTOR@@@Z, virtual) — a projectile
    // `actor` (pjl descriptor) is in flight; if its owner entity is the local player, boost the
    // grass rocket-awareness counter. REVERSED: src/ws/ai/aiPLANNER__implNoticeFlyingRocket.cpp.
    virtual void implNoticeFlyingRocket(struct iaIACTOR *actor);
    // 0x83164AB8 (const, virtual) — heuristic: is the local player's team facing a big battle
    // (more than 3 enemies). REVERSED: src/ws/ai/aiPLANNER__IsBigBattle.cpp.
    bool IsBigBattle() const;
    // 0x83164B30 (const) — interop-alert every active unit on `team` about an event at `posAlert`.
    // REVERSED: src/ws/ai/aiPLANNER__AlertTeam.cpp.
    void AlertTeam(const propGAME_INFO_TEAM &team, const m3dV &posAlert) const;

    // 0x83169DB8 — add the behavior-PS section named `secName` as a parent of `ps`; returns
    // true when that section exists. REVERSED: src/ws/ai/aiPLANNER__AddParentToSection.cpp.
    bool AddParentToSection(struct psSECTION &ps, const dsTSTRING<char> &secName);
    // 0x83169E28 (const) — count active unit watchers whose team matches `team`.
    // REVERSED: src/ws/ai/aiPLANNER__GetActiveUnitsCnt.cpp.
    int GetActiveUnitsCnt(const propGAME_INFO_TEAM &team) const;
    // 0x8316AD48 — sum GetBehaviorsCount() over every registered aiBRAIN (msg system iterate).
    // REVERSED: src/ws/ai/aiPLANNER__GetTotalBehaviorsCount.cpp.
    int GetTotalBehaviorsCount();
    // 0x8316ADD0 (virtual) — find the registered aiBRAIN whose on-screen debug index == `index`.
    // REVERSED: src/ws/ai/aiPLANNER__FindWithOnScreenIdx.cpp.
    struct aiBRAIN_IFACE *FindWithOnScreenIdx(int index);
    // 0x8316AE50 (const) — look up a behavior brand by descriptor name (null if `nameDesc` null).
    // REVERSED: src/ws/ai/aiPLANNER__FindBrand.cpp.
    struct dscBRAND *FindBrand(char *nameDesc) const;
    // 0x8316D908 (virtual) — register all built-in behavior brand descriptors (bvd* family).
    // REVERSED: src/ws/ai/aiPLANNER__RegisterBehaviorDescs.cpp.
    void RegisterBehaviorDescs();
    // 0x8316E540 — register every behavior brand's SSL script functions with `sslClass`.
    // REVERSED: src/ws/ai/aiPLANNER__RegisterBhvSslFunctions.cpp.
    void RegisterBhvSslFunctions(struct sslCLASS_REF &sslClass);

    // 0x831738B0 (protected) — set the attitude between the two named (gs-registered) teams;
    // returns true on success. REVERSED: src/ws/ai/aiPLANNER__SetTeamsAttitude.cpp.
    bool SetTeamsAttitude(const dsTSTRING<char> &teamIDFrom, const dsTSTRING<char> &teamIDTo,
                          gsTEAMS::ATTITUDE att);
    // 0x83173CD0 (virtual) — SSL predicate: are the two objects (entities) mutually neutral.
    // REVERSED: src/ws/ai/aiPLANNER__implIsFriendly.cpp.
    bool implIsFriendly(struct iaIACTOR *obj1, struct iaIACTOR *obj2);

    // --- nav-subsystem accessors (aiPLANNER_IFACE virtuals) used by aiEXIFACE ---  boundary
    // ?GetNavEditor@aiPLANNER@@UAAPAVnedNAV_EDITOR@@XZ
    nedNAV_EDITOR *GetNavEditor();
    // ?GetNavManager@aiPLANNER@@UBAPAVnavMANAGER@@XZ
    navMANAGER    *GetNavManager() const;
    // ?GetPenaltyManager@aiPLANNER@@UBAPAVnavPENMAN@@XZ — the dynamic-penalty manager. boundary.
    navPENMAN     *GetPenaltyManager() const;

    // 0x8319BCF8 (?ProcessDebugKeys@aiPLANNER@@IAAXXZ) — protected: poll the AI debug console keys
    // (clear/kill watchee, kill all/enemies, on-off AI, pick-cam-pos, create ally mate) and act.
    // REVERSED: src/ws/ai/aiPLANNER__ProcessDebugKeys.cpp.
    void ProcessDebugKeys();
    // ?DebugCreateAIEntity@aiPLANNER@@IAAXABV?$dsTSTRING@D@@0@Z — protected: spawn a debug AI entity
    // of behaviour `type` named `name`. Body external to this batch. boundary.
    void DebugCreateAIEntity(const dsTSTRING<char> &type, const dsTSTRING<char> &name);

    // 0x831947E0 (?ProcessOffAI@aiPLANNER@@QAAXPAVmsgADDR@@@Z) — if the render is enabled and
    // `pSend` is an aiBRAIN that is neither the debug-watchee nor already suspended, toggle it into
    // the off-AI (suspended) state. REVERSED: src/ws/ai/aiPLANNER__ProcessOffAI.cpp.
    void ProcessOffAI(struct msgADDR *pSend);

    // Look up nav waypoint `idx` (planner-interface accessor). Body external — boundary.
    struct navWP *GetWP(unsigned short idx);

    // vtbl (byte offset 0x1E8) — the planner's current HO-reconsider checker (an aiHO_SYS member-fn
    // pointer), supplied to aiGOAL_BASE::IsAcceptHO. Used by aiHO_SYS::AdvCheckAcceptHO. boundary.
    bool (aiHO_SYS::*ReconsiderHOChecker())(const m3dV &) const;

    // --- self-virtuals / helpers used by the above but defined elsewhere ---  boundary
    const m3dV *GetGravity(const m3dV *pos);          // iaIACTOR virtual
    // Planner-interface virtual: true when team `a` is hostile to team `b`. boundary — body
    // external to this batch (aiPLANNER_IFACE::IsEnemy).
    bool IsEnemy(const dsSTRID &a, const dsSTRID &b);

    // 0x8322ECB8 (?implFindGroupByName@aiPLANNER@@UAA?AVsslOBJ_REF@@ABV?$dsTSTRING@D@@@Z, virtual) —
    // look up a coordinated group by name via the embedded coordinator and return its SSL object
    // reference (empty when not found). REVERSED: aiPLANNER__implFindGroupByName.cpp.
    struct sslOBJ_REF implFindGroupByName(const dsTSTRING<char> &grpName);

    // --- per-frame / message dispatch (a8_plan_msg.cpp, reversed this batch) ---
    // 0x83246828 (?UpdateCursor@aiPLANNER@@IAAXXZ) — protected: shoot a debug cursor ray from the
    // debug camera and feed the hit to the nav editor / brain watchee.
    void UpdateCursor();
    // 0x832469F0 (?ProcessINIT@aiPLANNER@@UAAHPAX@Z) — virtual one-time init: behaviors, behav.ps,
    // teams, message subscriptions.
    int ProcessINIT(void *pInfo);
    // 0x83246B68 (?ProcessDebugFrame@aiPLANNER@@IAAXM@Z) — protected: per-frame debug processing.
    void ProcessDebugFrame(float time);
    // 0x83246D10 (?ProcessFRAME@aiPLANNER@@UAAXXZ) — virtual: drive every AI subsystem this frame.
    void ProcessFRAME();
    // 0x83246E70 (?ProcessRegisterTplNav@aiPLANNER@@IAAXPAVmsgADDR@@@Z) — protected: queue a newly
    // registered scene-template entity for nav processing.
    void ProcessRegisterTplNav(struct msgADDR *pSend);
    // 0x83246F40 (?ProcessUnRegisterTplNav@aiPLANNER@@IAAXPAVmsgADDR@@@Z) — protected: disconnect a
    // removed scene-template entity's waypoints.
    void ProcessUnRegisterTplNav(struct msgADDR *pSend);
    // 0x83247018 (?ProcessUnblockNavByRemovedInst@aiPLANNER@@IAAXPAVmsgADDR@@@Z) — protected: unblock
    // nav for a removed SMR-featured instance.
    void ProcessUnblockNavByRemovedInst(struct msgADDR *pSend);
    // 0x83247180 (?RenderDebugInfo@aiPLANNER@@UAAXPAUgsREND_DATA@@@Z) — virtual: render AI debug info.
    void RenderDebugInfo(struct gsREND_DATA *pRndData);
    // 0x83247330 (?ProcessMsg@aiPLANNER@@UAA?AW4msgRES@@HPAXPAVmsgADDR@@@Z) — virtual message handler.
    msgRES ProcessMsg(int msg, void *pInfo, struct msgADDR *pSend);
    // 0x83247430 (?DropAll@aiPLANNER@@IAAXXZ) — protected: drop all per-level AI state.
    void DropAll();
    // 0x832474D0 (?ProcessTERM_LEVEL@aiPLANNER@@UAAXXZ) — virtual per-level teardown.
    void ProcessTERM_LEVEL();
    // 0x832475C0 (?ProcessINIT_LEVEL@aiPLANNER@@UAAHXZ) — virtual per-level init.
    int ProcessINIT_LEVEL();
    // 0x832476E0 (?implNavLink@aiPLANNER@@UAAXABV?$dsTSTRING@D@@000@Z) — SSL op: create a scripted
    // nav link between two waypoints in nav-system `sysName`.
    void implNavLink(const dsTSTRING<char> &sysName, const dsTSTRING<char> &wpA,
                     const dsTSTRING<char> &wpB, const dsTSTRING<char> &smtObjName);
    // 0x832595D0 (?cbNavLink@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback: unbox
    // four strings from argv[0..3] and forward to implNavLink. REVERSED: aiPLANNER__cbNavLink.cpp.
    void cbNavLink(int argc, dsDATA *argv, dsDATA &retVal, struct sslOBJ_REF caller);
    // 0x832596F8 (?cbNavLinkSingle@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback:
    // unbox four strings from argv[0..3] and forward to implNavLinkSingle. REVERSED: aiPLANNER__cbNavLinkSingle.cpp.
    void cbNavLinkSingle(int argc, dsDATA *argv, dsDATA &retVal, struct sslOBJ_REF caller);
    // 0x83259820 (?cbNavUnLink@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback: unbox
    // three strings from argv[0..2] and forward to implNavUnLink. REVERSED: aiPLANNER__cbNavUnLink.cpp.
    void cbNavUnLink(int argc, dsDATA *argv, dsDATA &retVal, struct sslOBJ_REF caller);
    // 0x83259910 (?cbNavUnLinkSingle@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback: unbox
    // three strings from argv[0..2] and forward to implNavUnLinkSingle. REVERSED: aiPLANNER__cbNavUnLinkSingle.cpp.
    void cbNavUnLinkSingle(int argc, dsDATA *argv, dsDATA &retVal, struct sslOBJ_REF caller);
    // 0x83259A00 (?cbNavBlockObj@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback: unbox two
    // strings from argv[0..1], forward to implNavBlockObj, box its bool into retVal. REVERSED: aiPLANNER__cbNavBlockObj.cpp.
    void cbNavBlockObj(int argc, dsDATA *argv, dsDATA &retVal, struct sslOBJ_REF caller);
    // 0x83259AF8 (?cbNavUnBlockObj@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback: unbox two
    // strings from argv[0..1], forward to implNavUnBlockObj, box its bool into retVal. REVERSED: aiPLANNER__cbNavUnBlockObj.cpp.
    void cbNavUnBlockObj(int argc, dsDATA *argv, dsDATA &retVal, struct sslOBJ_REF caller);
    // 0x83259BF0 (?cbNavDisableWP@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback: unbox the
    // waypoint string argv[0], forward to implNavDisableWP, box its bool into retVal. REVERSED: aiPLANNER__cbNavDisableWP.cpp.
    void cbNavDisableWP(int argc, dsDATA *argv, dsDATA &retVal, struct sslOBJ_REF caller);
    // 0x83259CA8 (?cbNavEnableWP@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback: unbox the
    // waypoint string argv[0], forward to implNavEnableWP, box its bool into retVal. REVERSED: aiPLANNER__cbNavEnableWP.cpp.
    void cbNavEnableWP(int argc, dsDATA *argv, dsDATA &retVal, struct sslOBJ_REF caller);
    // 0x83259D60 (?cbNavForceAutolinkWP@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback: unbox
    // the waypoint string argv[0], forward to implNavForceAutolinkWP, box its bool into retVal. REVERSED: aiPLANNER__cbNavForceAutolinkWP.cpp.
    void cbNavForceAutolinkWP(int argc, dsDATA *argv, dsDATA &retVal, struct sslOBJ_REF caller);
    // 0x83259E18 (?cbNavRelinkWPToNS@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback: unbox two
    // strings from argv[0..1], forward to implNavRelinkWPToNS, box its bool into retVal. REVERSED: aiPLANNER__cbNavRelinkWPToNS.cpp.
    void cbNavRelinkWPToNS(int argc, dsDATA *argv, dsDATA &retVal, struct sslOBJ_REF caller);
    // 0x83259F10 (?cbNavSysMarkStatic@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback: unbox
    // string argv[0] + bool argv[1], forward to implNavSysMarkStatic, box its bool into retVal. REVERSED: aiPLANNER__cbNavSysMarkStatic.cpp.
    void cbNavSysMarkStatic(int argc, dsDATA *argv, dsDATA &retVal, struct sslOBJ_REF caller);
    // 0x83259FE0 (?cbNavSysPenalizeByDomain@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback: unbox
    // strings argv[0..1] + int argv[2], forward to implNavSysPenalizeByDomain, box its bool into retVal. REVERSED: aiPLANNER__cbNavSysPenalizeByDomain.cpp.
    void cbNavSysPenalizeByDomain(int argc, dsDATA *argv, dsDATA &retVal, struct sslOBJ_REF caller);
    // 0x83247CD0 (?implNavSysRemove@aiPLANNER@@UAA_NABV?$dsTSTRING@D@@@Z) — SSL op: tear down the
    // nav-system named `nsName` and re-localize every active unit that was on it. False when it does
    // not exist. REVERSED: src/ws/ai/aiPLANNER__implNavSysRemove.cpp.
    bool implNavSysRemove(const dsTSTRING<char> &nsName);
    // 0x83247DB8 (?implNavRelinkWPToNS@aiPLANNER@@UAA_NABV?$dsTSTRING@D@@0@Z) — SSL op: move waypoint
    // `wpName` into nav-system `nsName`. False (and logs) when the nav-system, waypoint, or relink
    // fails. REVERSED: src/ws/ai/aiPLANNER__implNavRelinkWPToNS.cpp.
    bool implNavRelinkWPToNS(const dsTSTRING<char> &wpName, const dsTSTRING<char> &nsName);
    // 0x83247FE8 (?implNavUnLink@aiPLANNER@@UAAXABV?$dsTSTRING@D@@00@Z) — SSL op: remove the scripted
    // (bidirectional) nav link between waypoints `wpA` and `wpB` in nav-system `sysName`. REVERSED:
    // src/ws/ai/aiPLANNER__implNavUnLink.cpp.
    void implNavUnLink(const dsTSTRING<char> &sysName, const dsTSTRING<char> &wpA,
                       const dsTSTRING<char> &wpB);
    // 0x832480E8 (?implNavUnLinkSingle@aiPLANNER@@UAAXABV?$dsTSTRING@D@@00@Z) — SSL op: remove the
    // single (one-directional) scripted nav link from `wpA` to `wpB` in `sysName`. REVERSED:
    // src/ws/ai/aiPLANNER__implNavUnLinkSingle.cpp.
    void implNavUnLinkSingle(const dsTSTRING<char> &sysName, const dsTSTRING<char> &wpA,
                             const dsTSTRING<char> &wpB);
    // 0x83247798 (?implNavLinkSingle@aiPLANNER@@UAAXABV?$dsTSTRING@D@@000@Z) — SSL op: create a single
    // (one-directional) scripted nav link from `wpA` to `wpB` in `sysName`, tagged `smtObjName`.
    // REVERSED: src/ws/ai/aiPLANNER__implNavLinkSingle.cpp.
    void implNavLinkSingle(const dsTSTRING<char> &sysName, const dsTSTRING<char> &wpA,
                           const dsTSTRING<char> &wpB, const dsTSTRING<char> &smtObjName);
    // 0x83247850 (?implNavBlockObj@aiPLANNER@@UAA_NABV?$dsTSTRING@D@@0@Z) — SSL op: block the nav graph
    // around sub-object `objName` of scene instance `instName`. REVERSED: aiPLANNER__implNavBlockObj.cpp.
    bool implNavBlockObj(const dsTSTRING<char> &instName, const dsTSTRING<char> &objName);
    // 0x832478D0 (?implNavUnBlockObj@aiPLANNER@@UAA_NABV?$dsTSTRING@D@@0@Z) — SSL op: inverse of
    // implNavBlockObj. REVERSED: src/ws/ai/aiPLANNER__implNavUnBlockObj.cpp.
    bool implNavUnBlockObj(const dsTSTRING<char> &instName, const dsTSTRING<char> &objName);
    // 0x83247950 (?implNavDisableWP@aiPLANNER@@UAA_NABV?$dsTSTRING@D@@@Z) — SSL op: disable waypoint
    // `wpName`. REVERSED: src/ws/ai/aiPLANNER__implNavDisableWP.cpp.
    bool implNavDisableWP(const dsTSTRING<char> &wpName);
    // 0x832479D0 (?implNavEnableWP@aiPLANNER@@UAA_NABV?$dsTSTRING@D@@@Z) — SSL op: enable waypoint
    // `wpName`. REVERSED: src/ws/ai/aiPLANNER__implNavEnableWP.cpp.
    bool implNavEnableWP(const dsTSTRING<char> &wpName);
    // 0x83247A50 (?implNavForceAutolinkWP@aiPLANNER@@UAA_NABV?$dsTSTRING@D@@@Z) — SSL op: enable and
    // force-autolink waypoint `wpName`. REVERSED: src/ws/ai/aiPLANNER__implNavForceAutolinkWP.cpp.
    bool implNavForceAutolinkWP(const dsTSTRING<char> &wpName);
    // 0x83247AE8 (?implNavSysMarkStatic@aiPLANNER@@UAA_NABV?$dsTSTRING@D@@_N@Z) — SSL op: set the static
    // flag on nav-system `nsName`. REVERSED: src/ws/ai/aiPLANNER__implNavSysMarkStatic.cpp.
    bool implNavSysMarkStatic(const dsTSTRING<char> &nsName, bool isStatic);
    // 0x83247B58 (?implNavSysPenalizeByDomain@aiPLANNER@@UAA_NABV?$dsTSTRING@D@@0H@Z) — SSL op: raise the
    // penalty of domain `nameDom` in nav-system `nameNS` by `level`. REVERSED:
    // src/ws/ai/aiPLANNER__implNavSysPenalizeByDomain.cpp.
    bool implNavSysPenalizeByDomain(const dsTSTRING<char> &nameNS, const dsTSTRING<char> &nameDom,
                                    int level);
    // 0x832488B0 (?implAddEnemy@aiPLANNER@@UAA_NPAViaIACTOR@@@Z) — SSL op: register `actor` as an
    // active enemy entity (rejects brains/players, which are already enemies). Always returns 0.
    // REVERSED: src/ws/ai/aiPLANNER__implAddEnemy.cpp.
    bool implAddEnemy(struct iaIACTOR *actor);
    // 0x83248958 (?implRemoveEnemy@aiPLANNER@@UAAXPAViaIACTOR@@@Z) — SSL op: unregister `actor` as an
    // active enemy entity (rejects brains/players). REVERSED: src/ws/ai/aiPLANNER__implRemoveEnemy.cpp.
    void implRemoveEnemy(struct iaIACTOR *actor);
    // 0x83173B38 (?implSetTeamsEnemies@aiPLANNER@@UAA_NABV?$dsTSTRING@D@@0@Z) — SSL op: make team
    // `teamFrom` hostile to team `teamTo`. boundary (next frontier).
    bool implSetTeamsEnemies(const dsTSTRING<char> &teamFrom, const dsTSTRING<char> &teamTo);
    // 0x83173B40 (?implSetTeamsNeutrals@aiPLANNER@@UAA_NABV?$dsTSTRING@D@@0@Z) — SSL op: make team
    // `teamFrom` neutral to team `teamTo`. boundary (next frontier).
    bool implSetTeamsNeutrals(const dsTSTRING<char> &teamFrom, const dsTSTRING<char> &teamTo);
    // 0x83264080 (?implCountLiveInDomain@aiPLANNER@@UAAHPAViaIACTOR@@ABV?$dsTSTRING@D@@@Z) — SSL op:
    // count live actors of team `team` inside domain object `dom`. boundary (next frontier).
    int implCountLiveInDomain(struct iaIACTOR *dom, const dsTSTRING<char> &team);
    // 0x83248890 (?implEnableStrike@aiPLANNER@@UAAXABV?$dsTSTRING@D@@@Z) — SSL op: enable strike
    // `nameStrike`. boundary (next frontier).
    void implEnableStrike(const dsTSTRING<char> &nameStrike);
    // 0x832488A0 (?implDisableStrike@aiPLANNER@@UAAXABV?$dsTSTRING@D@@@Z) — SSL op: disable strike
    // `nameStrike`. boundary (next frontier).
    void implDisableStrike(const dsTSTRING<char> &nameStrike);
    // 0x83263C48 (?impltSetFlag@aiPLANNER@@UAA_NABV?$dsTSTRING@D@@@Z) — SSL op: set team flag `flag`.
    // boundary (next frontier).
    bool impltSetFlag(const dsTSTRING<char> &flag);
    // 0x832618B0 (?impltClearFlag@aiPLANNER@@UAA_NABV?$dsTSTRING@D@@@Z) — SSL op: clear team flag
    // `flag`. boundary (next frontier).
    bool impltClearFlag(const dsTSTRING<char> &flag);
    // 0x83260628 (?impltIsFlag@aiPLANNER@@UAA_NABV?$dsTSTRING@D@@@Z) — SSL op: test team flag `flag`.
    // boundary (next frontier).
    bool impltIsFlag(const dsTSTRING<char> &flag);

    // --- SSL-emitter / callback pairs (a8_planner.cpp, reversed this batch) ---
    // These "emitter" methods marshal their C++ args into dsDATA and dispatch the matching
    // per-species SSL script function id cached in the descriptor (aiPLANNER_DESC::sslFuncId_*);
    // the paired cb* method is the native callback SSL invokes, which unpacks argv and forwards to
    // the impl* implementation.
    // These read their per-species SSL function id via aiPLANNER_DESC_sslfunc_boundary.h and
    // dispatch through sslObject.CallFunc. REVERSED: src/ws/ai/aiPLANNER__<Method>.cpp.
    // 0x83251C50 (?NavLink@aiPLANNER@@QAAXABV?$dsTSTRING@D@@000@Z)
    void NavLink(const dsTSTRING<char> &sysName, const dsTSTRING<char> &wpA,
                 const dsTSTRING<char> &wpB, const dsTSTRING<char> &wpC);
    // 0x83251F60 (?NavLinkSingle@aiPLANNER@@QAAXABV?$dsTSTRING@D@@000@Z)
    void NavLinkSingle(const dsTSTRING<char> &sysName, const dsTSTRING<char> &wpA,
                       const dsTSTRING<char> &wpB, const dsTSTRING<char> &wpC);
    // 0x83252270 (?NavUnLink@aiPLANNER@@QAAXABV?$dsTSTRING@D@@00@Z)
    void NavUnLink(const dsTSTRING<char> &sysName, const dsTSTRING<char> &wpA,
                   const dsTSTRING<char> &wpB);
    // 0x832524E0 (?NavUnLinkSingle@aiPLANNER@@QAAXABV?$dsTSTRING@D@@00@Z)
    void NavUnLinkSingle(const dsTSTRING<char> &sysName, const dsTSTRING<char> &wpA,
                         const dsTSTRING<char> &wpB);
    // 0x83252750 (?NavBlockObj@aiPLANNER@@QAA_NABV?$dsTSTRING@D@@0@Z)
    bool NavBlockObj(const dsTSTRING<char> &instName, const dsTSTRING<char> &objName);
    // 0x83252960 (?NavUnBlockObj@aiPLANNER@@QAA_NABV?$dsTSTRING@D@@0@Z)
    bool NavUnBlockObj(const dsTSTRING<char> &instName, const dsTSTRING<char> &objName);
    // 0x83252B70 (?NavDisableWP@aiPLANNER@@QAA_NABV?$dsTSTRING@D@@@Z)
    bool NavDisableWP(const dsTSTRING<char> &wpName);
    // 0x83252CE8 (?NavEnableWP@aiPLANNER@@QAA_NABV?$dsTSTRING@D@@@Z)
    bool NavEnableWP(const dsTSTRING<char> &wpName);
    // 0x83252E60 (?NavForceAutolinkWP@aiPLANNER@@QAA_NABV?$dsTSTRING@D@@@Z)
    bool NavForceAutolinkWP(const dsTSTRING<char> &wpName);
    // 0x83252FD8 (?NavRelinkWPToNS@aiPLANNER@@QAA_NABV?$dsTSTRING@D@@0@Z)
    bool NavRelinkWPToNS(const dsTSTRING<char> &wpName, const dsTSTRING<char> &nsName);
    // 0x832531E8 (?NavSysMarkStatic@aiPLANNER@@QAA_NABV?$dsTSTRING@D@@_N@Z)
    bool NavSysMarkStatic(const dsTSTRING<char> &nsName, bool isStatic);
    // 0x832533A8 (?NavSysPenalizeByDomain@aiPLANNER@@QAA_NABV?$dsTSTRING@D@@0H@Z)
    bool NavSysPenalizeByDomain(const dsTSTRING<char> &nameNS, const dsTSTRING<char> &nameDom,
                                int domain);
    // 0x83253600 (?NavSysRemove@aiPLANNER@@QAA_NABV?$dsTSTRING@D@@@Z)
    bool NavSysRemove(const dsTSTRING<char> &nsName);
    // 0x83253778 (?SetTeamsEnemies@aiPLANNER@@QAA_NABV?$dsTSTRING@D@@0@Z)
    bool SetTeamsEnemies(const dsTSTRING<char> &teamA, const dsTSTRING<char> &teamB);
    // 0x83253988 (?SetTeamsNeutrals@aiPLANNER@@QAA_NABV?$dsTSTRING@D@@0@Z) — SSL emitter: mark the
    // two named teams mutually neutral; returns the script result. REVERSED:
    // src/ws/ai/aiPLANNER__SetTeamsNeutrals.cpp.
    bool SetTeamsNeutrals(const dsTSTRING<char> &teamIDA, const dsTSTRING<char> &teamIDB);
    // 0x83253B98 (?IsFriendly@aiPLANNER@@QAA_NPAViaIACTOR@@0@Z) — SSL emitter: ask the script whether
    // the two actors are friendly. REVERSED: src/ws/ai/aiPLANNER__IsFriendly.cpp.
    bool IsFriendly(struct iaIACTOR *obj1, struct iaIACTOR *obj2);
    // 0x83253DA0 (?cbIsFriendly@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback for
    // IsFriendly: resolve both actor refs and forward to implIsFriendly, storing the bool result.
    // REVERSED: src/ws/ai/aiPLANNER__cbIsFriendly.cpp.
    void cbIsFriendly(int argc, dsDATA *argv, dsDATA &retVal, struct sslOBJ_REF caller);
    // 0x83253ED8 (?SetHOMaxAngleCorner@aiPLANNER@@QAAXM@Z) — SSL emitter: forward `angle` to the
    // script SetHOMaxAngleCorner. REVERSED: src/ws/ai/aiPLANNER__SetHOMaxAngleCorner.cpp.
    void SetHOMaxAngleCorner(float angle);
    // 0x83253FC0 (?cbSetHOMaxAngleCorner@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL
    // callback: unpack the float and forward to implSetHOMaxAngleCorner.
    // REVERSED: src/ws/ai/aiPLANNER__cbSetHOMaxAngleCorner.cpp.
    void cbSetHOMaxAngleCorner(int argc, dsDATA *argv, dsDATA &retVal, struct sslOBJ_REF caller);
    // 0x83254028 (?SetHOMaxAngleMiddle@aiPLANNER@@QAAXM@Z) — SSL emitter: forward `angle` to the
    // script SetHOMaxAngleMiddle. REVERSED: src/ws/ai/aiPLANNER__SetHOMaxAngleMiddle.cpp.
    void SetHOMaxAngleMiddle(float angle);
    // 0x83254110 (?cbSetHOMaxAngleMiddle@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL
    // callback: unpack the float and forward to implSetHOMaxAngleMiddle.
    // REVERSED: src/ws/ai/aiPLANNER__cbSetHOMaxAngleMiddle.cpp.
    void cbSetHOMaxAngleMiddle(int argc, dsDATA *argv, dsDATA &retVal, struct sslOBJ_REF caller);
    // 0x83254178 (?NoticeFlyingGrenade@aiPLANNER@@QAAXPAViaIACTOR@@@Z) — SSL emitter: notify the
    // script that `actor` (a grenade) is in flight. REVERSED: src/ws/ai/aiPLANNER__NoticeFlyingGrenade.cpp.
    void NoticeFlyingGrenade(struct iaIACTOR *actor);
    // 0x832542B0 (?cbNoticeFlyingGrenade@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL
    // callback: resolve the actor ref and forward to implNoticeFlyingGrenade.
    // REVERSED: src/ws/ai/aiPLANNER__cbNoticeFlyingGrenade.cpp.
    void cbNoticeFlyingGrenade(int argc, dsDATA *argv, dsDATA &retVal, struct sslOBJ_REF caller);
    // 0x83254348 (?NoticeGrenadeExplosion@aiPLANNER@@QAAXPAViaIACTOR@@@Z) — SSL emitter: notify the
    // script that grenade `actor` detonated. REVERSED: src/ws/ai/aiPLANNER__NoticeGrenadeExplosion.cpp.
    void NoticeGrenadeExplosion(struct iaIACTOR *actor);
    // 0x83254480 (?cbNoticeGrenadeExplosion@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL
    // callback: resolve the actor ref and forward to implNoticeGrenadeExplosion.
    // REVERSED: src/ws/ai/aiPLANNER__cbNoticeGrenadeExplosion.cpp.
    void cbNoticeGrenadeExplosion(int argc, dsDATA *argv, dsDATA &retVal, struct sslOBJ_REF caller);
    // 0x83254518 (?NoticeFlyingRocket@aiPLANNER@@QAAXPAViaIACTOR@@@Z) — SSL emitter: notify the
    // script that projectile `actor` is in flight. REVERSED: src/ws/ai/aiPLANNER__NoticeFlyingRocket.cpp.
    void NoticeFlyingRocket(struct iaIACTOR *actor);
    // 0x83254650 (?cbNoticeFlyingRocket@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL
    // callback: resolve the actor ref and forward to implNoticeFlyingRocket.
    // REVERSED: src/ws/ai/aiPLANNER__cbNoticeFlyingRocket.cpp.
    void cbNoticeFlyingRocket(int argc, dsDATA *argv, dsDATA &retVal, struct sslOBJ_REF caller);
    // 0x832546E8 (?RegisterDeathSmtObject@aiPLANNER@@QAAXPAViaIACTOR@@@Z) — SSL emitter: register
    // `actor` as an animated-death smart object with the script. REVERSED:
    // src/ws/ai/aiPLANNER__RegisterDeathSmtObject.cpp.
    void RegisterDeathSmtObject(struct iaIACTOR *actor);
    // 0x83254820 (?cbRegisterDeathSmtObject@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL
    // callback: resolve argv[0]'s host actor, forward to implRegisterDeathSmtObject.
    // REVERSED: src/ws/ai/aiPLANNER__cbRegisterDeathSmtObject.cpp.
    void cbRegisterDeathSmtObject(int argc, dsDATA *argv, dsDATA &retVal, struct sslOBJ_REF caller);
    // 0x832548B8 (?UnregisterDeathSmtObject@aiPLANNER@@QAAXPAViaIACTOR@@@Z) — SSL emitter: run
    // "UnregisterDeathSmtObject" with `actor`'s SSL ref. REVERSED: aiPLANNER__UnregisterDeathSmtObject.cpp.
    void UnregisterDeathSmtObject(struct iaIACTOR *actor);
    // 0x832549F0 (?cbUnregisterDeathSmtObject@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL
    // callback: forward argv[0]'s host actor to implUnregisterDeathSmtObject.
    // REVERSED: src/ws/ai/aiPLANNER__cbUnregisterDeathSmtObject.cpp.
    void cbUnregisterDeathSmtObject(int argc, dsDATA *argv, dsDATA &retVal, struct sslOBJ_REF caller);
    // 0x83254A88 (?FindGroupByName@aiPLANNER@@QAA?AVsslOBJ_REF@@ABV?$dsTSTRING@D@@@Z) — SSL emitter:
    // run "FindGroupByName" with `grpName`; resolve and return the returned group object ref.
    // REVERSED: src/ws/ai/aiPLANNER__FindGroupByName.cpp.
    struct sslOBJ_REF FindGroupByName(const dsTSTRING<char> &grpName);
    // 0x83254C48 (?CountLiveInDomain@aiPLANNER@@QAAHPAViaIACTOR@@ABV?$dsTSTRING@D@@@Z) — SSL emitter:
    // run "CountLiveInDomain" with (`actor`, `domain`); returns the int count.
    // REVERSED: src/ws/ai/aiPLANNER__CountLiveInDomain.cpp.
    int CountLiveInDomain(struct iaIACTOR *actor, const dsTSTRING<char> &domain);
    // 0x83254E58 (?EnableStrike@aiPLANNER@@QAAXABV?$dsTSTRING@D@@@Z) — SSL emitter: run "EnableStrike"
    // with `strikeName`. REVERSED: src/ws/ai/aiPLANNER__EnableStrike.cpp.
    void EnableStrike(const dsTSTRING<char> &strikeName);
    // 0x83254F98 (?DisableStrike@aiPLANNER@@QAAXABV?$dsTSTRING@D@@@Z) — SSL emitter: run "DisableStrike"
    // with `strikeName`. REVERSED: src/ws/ai/aiPLANNER__DisableStrike.cpp.
    void DisableStrike(const dsTSTRING<char> &strikeName);
    // 0x832550D8 (?AddEnemy@aiPLANNER@@QAA_NPAViaIACTOR@@@Z) — SSL emitter: run "AddEnemy" with
    // `actor`'s SSL ref; returns its bool result. REVERSED: src/ws/ai/aiPLANNER__AddEnemy.cpp.
    bool AddEnemy(struct iaIACTOR *actor);
    // 0x83255238 (?cbAddEnemy@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback: forward
    // argv[0]'s host actor to implAddEnemy, store the bool result. REVERSED: aiPLANNER__cbAddEnemy.cpp.
    void cbAddEnemy(int argc, dsDATA *argv, dsDATA &retVal, struct sslOBJ_REF caller);
    // 0x83255318 (?RemoveEnemy@aiPLANNER@@QAAXPAViaIACTOR@@@Z) — SSL emitter: run "RemoveEnemy" with
    // `actor`'s SSL ref. REVERSED: src/ws/ai/aiPLANNER__RemoveEnemy.cpp.
    void RemoveEnemy(struct iaIACTOR *actor);
    // 0x83255450 (?cbRemoveEnemy@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback:
    // forward argv[0]'s host actor to implRemoveEnemy. REVERSED: aiPLANNER__cbRemoveEnemy.cpp.
    void cbRemoveEnemy(int argc, dsDATA *argv, dsDATA &retVal, struct sslOBJ_REF caller);
    // 0x832554E8 (?tSetFlag@aiPLANNER@@QAA_NABV?$dsTSTRING@D@@@Z) — SSL emitter: run task-system
    // "SetFlag" with `flagName`; returns its bool result. REVERSED: src/ws/ai/aiPLANNER__tSetFlag.cpp.
    bool tSetFlag(const dsTSTRING<char> &flagName);
    // 0x83255660 (?tClearFlag@aiPLANNER@@QAA_NABV?$dsTSTRING@D@@@Z) — SSL emitter: run task-system
    // "ClearFlag" with `flagName`; returns its bool result. REVERSED: aiPLANNER__tClearFlag.cpp.
    bool tClearFlag(const dsTSTRING<char> &flagName);
    // 0x832557D8 (?tIsFlag@aiPLANNER@@QAA_NABV?$dsTSTRING@D@@@Z) — SSL emitter: run task-system
    // "IsFlag" with `flagName`; returns its bool result. REVERSED: src/ws/ai/aiPLANNER__tIsFlag.cpp.
    bool tIsFlag(const dsTSTRING<char> &flagName);

    // --- additional SSL callbacks registered by aiPLANNER_DESC::InitSslClass (this batch) ---
    // 0x8325A0F0 (?cbNavSysRemove@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback:
    // unbox argv[0] (nav-system name), forward to implNavSysRemove, store the bool result.
    void cbNavSysRemove(int argc, dsDATA *argv, dsDATA &retVal, struct sslOBJ_REF caller);
    // 0x8325A1A8 (?cbSetTeamsEnemies@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback:
    // unbox argv[0]/argv[1] (team names), forward to implSetTeamsEnemies, store the bool result.
    void cbSetTeamsEnemies(int argc, dsDATA *argv, dsDATA &retVal, struct sslOBJ_REF caller);
    // 0x8325A2A0 (?cbSetTeamsNeutrals@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback:
    // unbox argv[0]/argv[1] (team names), forward to implSetTeamsNeutrals, store the bool result.
    void cbSetTeamsNeutrals(int argc, dsDATA *argv, dsDATA &retVal, struct sslOBJ_REF caller);
    // 0x8325A398 (?cbFindGroupByName@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback:
    // unbox argv[0] (group name), forward to implFindGroupByName, store the returned object ref.
    void cbFindGroupByName(int argc, dsDATA *argv, dsDATA &retVal, struct sslOBJ_REF caller);
    // 0x8325A4A8 (?cbCountLiveInDomain@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback:
    // resolve argv[0]'s host actor (domain) + unbox argv[1] (team), forward to implCountLiveInDomain,
    // store the int result.
    void cbCountLiveInDomain(int argc, dsDATA *argv, dsDATA &retVal, struct sslOBJ_REF caller);
    // 0x8325A5C0 (?cbEnableStrike@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback:
    // unbox argv[0] (strike name), forward to implEnableStrike (no return value).
    void cbEnableStrike(int argc, dsDATA *argv, dsDATA &retVal, struct sslOBJ_REF caller);
    // 0x8325A648 (?cbDisableStrike@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback:
    // unbox argv[0] (strike name), forward to implDisableStrike (no return value).
    void cbDisableStrike(int argc, dsDATA *argv, dsDATA &retVal, struct sslOBJ_REF caller);
    // 0x8325A6D0 (?cbtSetFlag@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback:
    // unbox argv[0] (flag name), forward to impltSetFlag, store the bool result.
    void cbtSetFlag(int argc, dsDATA *argv, dsDATA &retVal, struct sslOBJ_REF caller);
    // 0x8325A788 (?cbtClearFlag@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback:
    // unbox argv[0] (flag name), forward to impltClearFlag, store the bool result.
    void cbtClearFlag(int argc, dsDATA *argv, dsDATA &retVal, struct sslOBJ_REF caller);
    // 0x8325A840 (?cbtIsFlag@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z) — SSL callback:
    // unbox argv[0] (flag name), forward to impltIsFlag, store the bool result.
    void cbtIsFlag(int argc, dsDATA *argv, dsDATA &retVal, struct sslOBJ_REF caller);

    // --- impl* targets of the cb* callbacks above (bodies external to this batch) ---  boundary
    // Trivial setters/notifiers invoked from the cb* callbacks; separate symbols, not reversed here.
    void implSetHOMaxAngleCorner(float angle);
    void implSetHOMaxAngleMiddle(float angle);
    void implNoticeFlyingGrenade(struct iaIACTOR *actor);
    // 0x8325F590 (?implRegisterDeathSmtObject@aiPLANNER@@UAAXPAViaIACTOR@@@Z, virtual) — worker for
    // cbRegisterDeathSmtObject. Next frontier.
    void implRegisterDeathSmtObject(struct iaIACTOR *actor);
    // 0x8325F348 (?implUnregisterDeathSmtObject@aiPLANNER@@UAAXPAViaIACTOR@@@Z, virtual) — worker for
    // cbUnregisterDeathSmtObject. Next frontier.
    void implUnregisterDeathSmtObject(struct iaIACTOR *actor);

    // --- siblings called by the dispatch methods (bodies external to this batch) ---  boundary
    // 0x... (?ProcessSound@aiPLANNER@@...) — handle an entity sound notice.
    void ProcessSound(const struct entSND_NOTICE *pNotice);
    // 0x... (?SelfRegisterForMessages@aiPLANNER@@...) — (re)subscribe to the message system.
    void SelfRegisterForMessages();
};
