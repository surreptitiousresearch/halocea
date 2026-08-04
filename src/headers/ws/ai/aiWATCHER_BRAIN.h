#pragma once
#include "../ds/WEAK_PTR.h"
#include "../ds/dsSTRID_FLAGS.h"
#include "../m3d/m3dV.h"
#include "../m3d/m3dMATR.h"

// ws-engine ai08 — a watcher bound to a specific AI unit (brain).
// DB-verified layout (types_members aiWATCHER_BRAIN) — size 100 (0x64):
//   aiWATCHER base@0 (76B, typed — aiWATCHER.h), brain@76, cachedReticleDir@80,
//   cachedReticleStart@92.
#include "aiWATCHER.h"

struct aiBRAIN;            // ai08 — per-unit AI brain   boundary
struct entENTITY;          // entENTITY.h
struct navSYS;             // nav subsystem              boundary
struct gsDOM_SPAWN;        // gs subsystem — spawn domain          boundary
struct propGAME_INFO_TEAM; // propGAME_INFO_TEAM.h
struct iaIACTOR;           // ws — interactive actor     boundary

struct aiWATCHER_BRAIN : aiWATCHER {         // base 0x00..0x4C (DB-typed; base carries the
                                             // explicit __vftable — decls here stay NON-virtual,
                                             // cdtREFINE precedent)
    ds::WEAK_PTR<aiBRAIN> brain;             // 0x4C weak ref to the tracked brain
    m3dV                 cachedReticleDir;   // 0x50
    m3dV                 cachedReticleStart; // 0x5C

    // --- methods reversed in this batch (a8_plan_wah.cpp) ---
    void UpdateReticleData();                   // 0x8316E850 (protected)
    m3dV GetReticle(m3dV &eye) const;           // 0x8316E8E0 (virtual const)
    static bool IsAcceptActor(iaIACTOR *actor); // 0x8316EE68 (static)
    bool Update(float dt);                      // 0x8316EF90 (virtual)
    // virtuals used by UpdateReticleData, defined elsewhere (ai08).  boundary
    m3dV *GetBodyPos(m3dV *out);
    m3dV *GetFaceDir(m3dV *out);

    // Defined elsewhere (ai08); used by aiPLANNER.  boundary
    bool    Is(const entENTITY *ent);
    // DEVIATION: returns propGAME_INFO_TEAM* (points at the base watcher's cached team), not int —
    // callers read ->team.id (aiPLANNER::GetCountEnemiesTo). Corrected from an earlier `int` guess.
    propGAME_INFO_TEAM *GetTeamID();
    // 0x8316F9F8 (?GetNavSystem@aiWATCHER_BRAIN@@QBAPAVnavSYS@@XZ) — forward to the brain's nav
    // system (null when invalid). const (DB QBA); an earlier decl guessed non-const.
    navSYS *GetNavSystem() const;
    // 0x8316FF80 — true when the brain's best behaviour is a (major) smart behaviour.
    bool    IsInSmartBHV() const;

    // Virtual — true when this watcher is a brain (unit) watcher. Used by aiMIND::CalcKeepDefCDTReqTime.
    // boundary — body external to this batch.
    bool         IsBrain() const;
    // Virtual — true when the tracked brain is dead. Used by aiPLANNER::CountLiveSpawnedFrom.
    bool         IsDead() const;
    // 0x831700E8 — the spawn domain this unit was spawned from (null if invalid). Used by
    // CountLiveSpawnedFrom.
    gsDOM_SPAWN *GetDomSpawn() const;
    // 0x8316FC00 — forward a grenade-terminated notification to the brain's perception module.
    // Used by aiPLANNER::NoticeGrenadeTerm.
    void         NoticeGrenadeTerm(entENTITY *grenade) const;
    // 0x8328F4C0 (?DbgAppendProps@aiWATCHER_BRAIN@@QBAXAAV?$dsTSTRING@D@@@Z) — append a tab-separated
    // human-readable dump of the aiWATCHER-base FSM property flags to `line` (aiDEBUG overlay).
    void         DbgAppendProps(dsTSTRING<char> &line) const;

    // --- methods reversed in the saber2 drain (a8_plan_wah.cpp) ---
    // 0x83170568 (?IsValid@aiWATCHER_BRAIN@@UBA_NXZ) — virtual const: true when the weak brain ref
    // is still live. Body external to this batch.  boundary
    bool IsValid() const;            // vtbl (aiWATCHER::IsValid)
    // 0x83172A60 (virtual const) — fill `flags` with the brain description's strike-immune set.
    void FillStrikeImmune(dsSTRID_FLAGS &flags) const;
    // 0x83172B00 (virtual) — write the current hideout snap matrix into `mSnap`; true if the
    // tracked model has a live hideout ITRC (else identity). Deep model access is ai08 boundary.
    bool GetHOMatrSnap(m3dMATR &mSnap);
    // 0x83172C78 (virtual const) — return the (negated, normalised) hideout direction from the
    // model base matrix (m3dVZero if invalid, unitX if unnormalisable). Deep model access boundary.
    m3dV GetHideoutDir() const;

    // 0x83170160 (virtual const) — negated, normalised reticle-space evade direction read from the
    // tracked brain's aiSTATUS (falls back to a secondary status vector when degenerate). Returns
    // an m3dV by value; posEvader is accepted for the interface but unused by this override.
    m3dV GetPreferredEvadeDir(const m3dV &posEvader) const;

    // 0x83170220 (virtual) — forward an on-path notification to the brain's "best" mind module,
    // unless the brain's aiSTATUS suppression bit (bit 2 from MSB of the flag word) is set.
    void NoticeBeingOnPath(const m3dV &dirOnPath);

    // 0x831702B0 (virtual const) — max simultaneous melee attackers from the unit descriptor
    // (888 when the watcher is no longer valid).
    int GetMaxMeleeEnemies() const;

    // 0x83170368 (virtual const) — max simultaneous ranged attackers from the unit descriptor
    // (888 when the watcher is no longer valid).
    int GetMaxRangedEnemies() const;

    // --- thin brain-forwarding accessors (0x8316FA80..0x83170060) ---

    // 0x8316FA80 — true when the brain has posted a "terminate" request (state bit 0x8); also true
    // (fail-safe) when the brain ref is invalid.
    bool IsTermPosted() const;

    // 0x8316FB00 — the tracked brain's grenade-avoidance radius (0 when invalid).
    float GetRadiusAvoidGrenade() const;

    // 0x8316FB80 — forward a "grenade noticed" notification to the brain's perception module.
    void NoticeGrenade(entENTITY *grenade) const;

    // 0x8316FC80 — virtual: true when this unit should wait (rather than push) to resolve being
    // stuck against unit `uidOther`: it is stationary, has the higher unique id, and is pathing.
    bool CanAvoidStuckByWaiting(int uidOther) const;

    // 0x8316FD30 — virtual: true when the brain's active weapon is currently firing.
    bool IsShooting() const;

    // 0x8316FE00 — virtual: true when the brain's mind is idle (also true when invalid).
    bool IsIdle() const;

    // 0x8316FE80 — true when the brain's status "optimizable" flag (gen bit 0x10) is set.
    bool IsOptimizable() const;

    // 0x8316FF00 — true when the brain's status "enemy visible now" flag (est bit 0x10) is set.
    bool IsEnemyVisibleNow() const;

    // 0x83170060 — true when `ent` is the brain's current enemy.
    bool HasEnemy(const entENTITY *ent) const;

    // --- brain body/status forwarding accessors reversed in the saber2 drain (0x8316F160..) ---
    // aiWATCHER base virtuals (aiWATCHER::IsHideoutMode / IsWlnMode, both UBA _N const). Bodies
    // external to this batch.  boundary
    bool IsHideoutMode() const;  // 0x8316ED68 (aiWATCHER)
    bool IsWlnMode() const;      // 0x8316EDE8 (aiWATCHER)

    // 0x8316F160 (?GetBodyPosNav@...@UBA?AUm3dV@@XZ) — virtual const: the brain's nav-space body
    // position (m3dVZero when invalid).
    m3dV GetBodyPosNav() const;
    // 0x8316F208 (virtual const) — write the brain's nav-space body matrix into `matr` (identity
    // when invalid).
    void GetBodyMatrNav(m3dMATR &matr) const;
    // 0x8316F2D0 (virtual const) — the brain's body coordinate system (position + forward + up);
    // falls back to (origin, +Z, +Y) when invalid.
    void GetBodyCS(m3dV &pos, m3dV &dir, m3dV &dirUp) const;
    // 0x8316F448 (virtual const) — the brain's face position & direction from its status snapshot
    // (origin, +Z when invalid).
    void GetFaceParams(m3dV &pos, m3dV &dir) const;
    // 0x8316F518 (virtual const) — true when in a low-hideout (hideout mode, not wall-lean, and the
    // brain status body flag 0x1000 set).
    bool IsInLowHO() const;
    // 0x8316F5E0 / 0x8316F6E0 (virtual const) — true when the brain body's hideout-position prop set
    // marks the unit on the left / right (outer) side of the hideout.
    bool IsHOSideLeft() const;
    bool IsHOSideRight() const;
    // 0x8316F7E0 (?GetUID@...@UBAKXZ) — virtual const: the tracked brain's unique id (0xFFFFFFFF
    // when invalid).
    unsigned int GetUID() const;
    // 0x8316F8E8 (?GetWannaBePos@...@QAAX...) — write the position the brain's "best" module wants
    // to reach into `pos` (m3dVUndef when invalid).
    void GetWannaBePos(m3dV &pos);
    // 0x8316F990 (?MarkToReduceVision@...@QAAXXZ) — request the brain's perception module shrink its
    // vision cone next tick.
    void MarkToReduceVision();

    // ?GetMinHeightFlesh@aiWATCHER@@UBAMXZ (inherited) — virtual, const: the watcher's min flesh
    // height above feet. Used by aiBRAIN::GetMinHeightFlesh.  boundary — body external to this batch.
    float GetMinHeightFlesh() const;
    // ?GetHeight@aiWATCHER@@UBAMXZ (inherited) — virtual, const: the watcher's full body height.
    // Used by aiBRAIN::GetHeight.  boundary — body external to this batch.
    float GetHeight() const;
};
