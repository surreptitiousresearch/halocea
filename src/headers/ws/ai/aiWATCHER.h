#pragma once
#include "../ds/dsFLAGS.h"
#include "../m3d/m3dV.h"
#include "../prop/propGAME_INFO_TEAM.h"

// ws-engine ai08: base watcher (used directly for player watchers, aiWAH_SYS::arrPlayers, and as
// the base of aiWATCHER_BRAIN). DB-verified layout (types_members aiWATCHER) — size 76 (0x4C).

enum AI_FSM_PROPS;          // ai08 — FSM property flags (bit meanings are a boundary)
struct propGAME_INFO;       // prop subsystem — game info                          boundary (ptr)
struct iaIACTOR;            // ia subsystem — interactive actor                    boundary (fwd)
struct entENTITY;           // ent subsystem — game entity                         boundary (fwd)
struct aiBRAIN;             // ai08 — per-unit AI brain                             boundary (fwd)
struct wpnLIST_BASE;        // wpn subsystem — a unit's weapon list                 boundary (fwd)

struct aiWATCHER {
    void *__vftable;                         // 0x00 aiWATCHER_vtbl*
    dsFLAGS<AI_FSM_PROPS, int> fsmPropFlags; // 0x04 translated STRID FSM property flags
    float timeOpen;                          // 0x08
    float timeInHO;                          // 0x0C
    float timeInHOMode;                      // 0x10
    float timerLogicalShield;                // 0x14
    int   countMeleeEnemies;                 // 0x18
    int   countRangedEnemies;                // 0x1C
    m3dV  hideoutDirLastUsed;                // 0x20
    float cachedMinHeightFlesh;              // 0x2C
    float cachedHeight;                      // 0x30
    propGAME_INFO_TEAM cachedteamID;         // 0x34
    bool  mayUpdateHeights;                  // 0x3C
    float minHeightFlesh;                    // 0x40
    float height;                            // 0x44
    propGAME_INFO *pGameInfo;                // 0x48

    // Virtuals / helpers defined elsewhere (ai08); used by aiPLANNER / aiWTC_HOLDER.  boundary
    propGAME_INFO_TEAM *GetTeamID();         // return this watcher's team
    unsigned int GetUID();                   // this watcher's unique id
    iaIACTOR *GetActor();                    // virtual — the actor this watcher tracks  boundary
    // Virtual (aiWATCHER_vtbl+0x34) — write the watched body's world position into `out`.
    // Used by aiBRAIN::implCountMyNeutralsAroundEnemy.  boundary.
    m3dV *GetBodyPos(m3dV *out);

    // Virtual (aiWATCHER_vtbl+0x38) — write the watched body's world-space up vector into `out`.
    // Used by aiFILT_ENEMY::PostMultipleRequest (visibility-check dir-up sample).  boundary.
    m3dV *GetBodyDirUp(m3dV *out);

    // Virtual — write the watched body's coordinate system (position + forward + up) into the three
    // out args. Used by aiDEBUG::RenderEnemyGroup (enemy-CS-from-watcher visualisation).  boundary.
    void GetBodyCS(m3dV *outPos, m3dV *outDir, m3dV *outUp);

    // Virtual (aiWATCHER_vtbl+0x3C) @0x8328D858 — the watched body's world-space right vector
    // (forward x up from GetBodyCS). REVERSED: src/ws/ai/aiWATCHER__GetBodyRight.cpp
    m3dV *GetBodyRight(m3dV *out);

    // Virtual (aiWATCHER_vtbl+0x44) @0x8328D918 — the watched body's face/eye world position
    // (position out of GetFaceParams). REVERSED: src/ws/ai/aiWATCHER__GetFacePos.cpp
    m3dV *GetFacePos(m3dV *out);

    // Virtual (aiWATCHER_vtbl+0x40, pure in base) — write the watched face coordinate frame
    // (position + direction) into the two out args. Overridden per watcher subclass.  boundary.
    void GetFaceParams(m3dV *outPos, m3dV *outDir);

    // 0x8326D418 (?IsInHighHO@aiWATCHER@@QBA_NXZ) — in hideout mode but not in the low hideout
    // slot. REVERSED: src/ws/ai/aiWATCHER__IsInHighHO.cpp
    bool IsInHighHO() const;   // IsInLowHO() declared below (aiWATCHER_vtbl+0x9C)

    // 0x8328D718 (?FillHeightsWithProp@aiWATCHER@@IAAXXZ) — refresh height / minHeightFlesh from the
    // tracked propGAME_INFO (0.9x height, defaults 0.1 / 0.2 when no game info).
    // REVERSED: src/ws/ai/aiWATCHER__FillHeightsWithProp.cpp
    void FillHeightsWithProp();

    // Virtuals (aiWATCHER_vtbl) reached by aiWAH_SYS. Bodies are ai08 boundary.
    bool Is(const iaIACTOR *actor);          // vtbl+0x4
    bool IsValid();                          // vtbl+0x8
    bool IsHideoutMode() const;              // ?IsHideoutMode@aiWATCHER@@UBA_NXZ — virtual  boundary
    bool IsMiniZoomMode() const;             // virtual — target is in mini-zoom mode  boundary
    bool IsShooting() const;                 // virtual — target is currently shooting  boundary
    // 0x8328E370 (?CanAcceptEnemy@aiWATCHER@@UBA_NPBVaiBRAIN@@@Z) — virtual const: this watcher
    // still has room (below its max) for another melee/ranged attacker like `enemy`.
    // REVERSED: src/ws/ai/aiWATCHER__CanAcceptEnemy.cpp
    bool CanAcceptEnemy(const aiBRAIN *enemy) const;
    bool IsEntity();                         // vtbl+0x14
    bool IsPlayer() const;                    // ?IsPlayer@aiWATCHER@@UBA_NXZ @0x8316E5E0 — virtual
    entENTITY *GetEntity() const;             // virtual const — the watched entity (?GetEntity@...UBAPAVentENTITY@@XZ)
    bool IsDead() const;                      // virtual const — the watched target is dead  boundary
    // 0x8328FDF8 (?Update@aiWATCHER@@UAA_NM@Z) — virtual per-frame tick: advance the hideout/open
    // timers, refresh game-info props and cached heights. REVERSED: src/ws/ai/aiWATCHER__Update.cpp
    bool Update(float dt);                   // vtbl+0x24 — per-frame tick
    // Virtual const (vtbl) — the hideout-facing direction (overridden per watcher subclass). boundary.
    m3dV GetHideoutDir() const;
    // 0x8328F930 (?FillGameInfoProp@aiWATCHER@@IAAXXZ) — lazily resolve and cache the watched
    // entity's propGAME_INFO into pGameInfo (scanning the property container when not the root).
    // REVERSED: src/ws/ai/aiWATCHER__FillGameInfoProp.cpp
    void FillGameInfoProp();

    // Virtual (aiWATCHER_vtbl+0x60) — distance from the body position along the dir-up vector to
    // the visibility-check sample point (the target's effective "head"). Used by
    // aiFILT_ENEMY::GetVisCheckParams.  boundary.
    float GetVisCheckSampleDist();

    // 0x8328F408 (?CalcAdjustVisChk@aiWATCHER@@QBA?AUm3dV@@PBVaiBRAIN@@@Z) — per-viewer adjustment
    // added to the visibility-check target offset (returns m3dV by value).  boundary.
    m3dV CalcAdjustVisChk(const aiBRAIN *viewer) const;

    // 0x8328F138 / 0x8328F268 — vertical / horizontal hideout VC (view-cover) adjustment vectors
    // (returns m3dV by value). REVERSED: src/ws/ai/aiWATCHER__Calc*VCAdjustHO.cpp.
    m3dV CalcVerticalVCAdjustHO(const aiBRAIN *observer) const;
    m3dV CalcHorizontalVCAdjustHO(const aiBRAIN *observer) const;

    // Additional watcher virtuals reached through the base by the Calc* helpers. Bodies are ai08
    // (overridden per-derived: aiWATCHER_PLAYER etc.). GetBodyRight is declared above.
    bool IsInLowHO() const;                  // aiWATCHER_vtbl+0x9C
    bool IsHOSideLeft() const;               // aiWATCHER_vtbl+0xA0
    bool IsHOSideRight() const;              // aiWATCHER_vtbl+0xA4

    // --- saber2 drain batch (aiWATCHER accessors @0x8328D9A0..0x8328E420) ---
    // 0x8328D9A0 (?GetFaceDir@aiWATCHER@@UBA?AUm3dV@@XZ) — virtual const: the direction component
    // of GetFaceParams (the position component is discarded). REVERSED: src/ws/ai/aiWATCHER__GetFaceDir.cpp
    m3dV GetFaceDir() const;
    // 0x8328DA28 (?GetSpeed@aiWATCHER@@UBA?AUm3dV@@XZ) — virtual const: the tracked entity's velocity
    // (m3dVZero when the watcher is invalid). REVERSED: src/ws/ai/aiWATCHER__GetSpeed.cpp
    m3dV GetSpeed() const;
    // Virtual (aiWATCHER_vtbl+0x20) — the tracked unit's weapon list (null if none). boundary.
    wpnLIST_BASE *GetWeaponList();
    // 0x8328DAE8 (?IsBigGun@aiWATCHER@@UAA_NXZ) — virtual: the current weapon is a rocket-launcher
    // variant. REVERSED: src/ws/ai/aiWATCHER__IsBigGun.cpp
    bool IsBigGun();
    // 0x8328E130 (?UpdateHeights@aiWATCHER@@IAAXXZ) — recompute cached body height / min-flesh height
    // from the tracked entity's skeleton (head / stomach bones). REVERSED: src/ws/ai/aiWATCHER__UpdateHeights.cpp
    void UpdateHeights();
    // 0x8328E298 / 0x8328E320 (?AddEnemy@ / ?RemoveEnemy@aiWATCHER@@UAAXPBVaiBRAIN@@@Z) — virtual:
    // bump / decrement the melee or ranged attacker count for `enemy` (by its aiSTATUS mind flag).
    // REVERSED: src/ws/ai/aiWATCHER__AddEnemy.cpp, aiWATCHER__RemoveEnemy.cpp
    void AddEnemy(const aiBRAIN *enemy);
    void RemoveEnemy(const aiBRAIN *enemy);
    // Virtuals — current melee/ranged attacker counts and their caps. boundary (overridden).
    int GetMeleeEnemies();                   // aiWATCHER_vtbl+0x68
    int GetRangedEnemies();                  // aiWATCHER_vtbl+0x70
    int GetMaxMeleeEnemies();                // aiWATCHER_vtbl+0x6C
    int GetMaxRangedEnemies();               // aiWATCHER_vtbl+0x74
    // Virtual const — target is in wall-lean (WLN) mode. boundary (used by IsInLowHO override).
    bool IsWlnMode() const;                  // aiWATCHER_vtbl+0xD4

    // Rebuild fsmPropFlags from the tracked body's cached property set. ai08 boundary.
    void TranslateSTRID_FLAGS(const struct ctrlPROP_LIST *propCache);

    // Factory / teardown (static, ai08). boundary.
    static aiWATCHER *Create(iaIACTOR *actor);
    static void       Destroy(aiWATCHER *w);
};
