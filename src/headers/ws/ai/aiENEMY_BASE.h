#pragma once
// ws-engine ai08: base of the brain "enemy-tracking" mind module (aiMIND.enemy pointee).
// DB-verified size 0x4 (single vtable pointer). Only the polymorphic query surface reached by the
// debug renderer is declared; the concrete arms (aiENEMY / aiENEMY_ZERO / aiENEMY_FROZEN) and the
// full vtable are the next frontier. boundary.

struct m3dV;
struct animINST;
struct aiWATCHER;
struct entENTITY;
struct aiENEMY_BASE_vtbl;

struct aiENEMY_BASE {
    aiENEMY_BASE_vtbl *__vftable; // 0x00

    // ?Is@aiENEMY_BASE@@UBA_NPBVentENTITY@@@Z — virtual, const: true when this module currently
    // tracks `ent` as its enemy (nullptr = "no enemy"). DB CORRECTION: previously mistyped here as
    // Is(const aiENEMY_BASE*); the DB mangling proves the sole overload takes const entENTITY*.
    bool Is(const entENTITY *ent) const;
    // true for the null/"zero" enemy arm.
    bool IsZero() const;
    // ?IsPlayer@aiENEMY_BASE@@UBA_NXZ @ 0x832B4660 — virtual, const: true when the tracked enemy
    // is the local player. boundary.
    bool IsPlayer() const;
    // vtbl 0x30 — virtual, const: true when the tracked enemy is an AI brain. Used by
    // aiBRAIN::GetEnemyType_SSL.  boundary — body external to this batch.
    bool IsBrain() const;
    // vtbl 0x38 — virtual, const: true when the tracked enemy is a plain entity. Used by
    // aiBRAIN::GetEnemyType_SSL.  boundary — body external to this batch.
    bool IsEntity() const;
    // vtbl — seconds the tracked enemy has been continuously invisible to this brain. Used by
    // aiBRAIN::GetFloatSSL (GETF_TIME_ENEMY_INVISIBLE).  boundary — body external to this batch.
    float GetTimeInvisible() const;
    // vtbl — seconds the tracked enemy has been in a hideout. Used by aiBRAIN::GetFloatSSL
    // (GETF_TIME_ENEMY_IN_HO).  boundary — body external to this batch.
    float GetTimeInHO() const;

    // the tracked enemy's animated instance (may be null).
    animINST *GetInst() const;
    // the watcher observing the tracked enemy (may be null).
    aiWATCHER *GetPeeker() const;

    // the tracked enemy entity (may be null). Used by aiMIND::InternalAssignEnemy.  boundary.
    const entENTITY *GetEntity() const;
    // this enemy module's team handle (points at a propGAME_INFO_TEAM). Used by
    // aiMIND::InternalAssignEnemy for the neutral-team status bit.  boundary.
    struct propGAME_INFO_TEAM *GetTeamID() const;
    // (re)initialise this enemy module; `isVisible` seeds the initial visibility state.  boundary.
    void Init(int isVisible);
    // queue this frame's visibility query for the tracked enemy. Used by aiMIND::UpdateAllPerception.
    void PrepareVisibilityRequest();
    // per-frame enemy-tracking update. Used by aiMIND::UpdateAllPerception.  boundary.
    void ProcessFrame();
    // vtbl 0x6C — fill the enemy coordinate snapshot for this frame. Used by
    // aiMIND::ProcessFrameBeforeBody.  boundary.
    void FillCoords();
    // true for the "frozen" enemy arm (aiENEMY_FROZEN).
    bool IsFrozen() const;
    // true when the tracked enemy is certainly dead (drives aiMIND::ValidateEnemySystem→ZeroEnemy).
    bool IsDeadForSure() const;
    // ?IsDead@aiENEMY_BASE@@UBA_NXZ — virtual, const: true when the tracked enemy is dead (vtbl 0x3C).
    // boundary — body external to this batch.
    bool IsDead() const;

    // Recompute the enemy-selection flag set for this module. Used by aiMIND::ProcessINITLive.
    // boundary — body external to this batch.
    void SetSelectionFlags();

    // ?UpdateTimers@aiENEMY_BASE@@UAAXM@Z — virtual: advance this module's timers by `dt` seconds.
    // Used by aiMIND::UpdateTimers.  boundary.
    void UpdateTimers(float dt);

    // vtbl 13 — true when this module is currently "at war" with the tracked enemy. Used by
    // aiPERC::ApplySingleSelectionWarPeace.  boundary (body external).
    bool IsInWar();
    // vtbl 36 — lock out enemy re-selection for `lockTime` seconds. Used by
    // aiPERC::NoticeHeavyDamaged.  boundary (body external).
    void LockChange(float lockTime);
};
