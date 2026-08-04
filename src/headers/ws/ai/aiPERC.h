#pragma once
#include "aiPERC_BASE.h"          // aiPERC_BASE (vtbl base of the perception module)
#include "../ds/dsFLAGS.h"        // state bitflags
#include "../ds/MAP.h"            // injury.wounds hash map (ds::MAP / ds::HASH / ds::CMP / allocator)
#include "../ds/WEAK_PTR.h"       // WOUND::shooter weak reference
#include "../ds/dsTSTRING.h"      // SetPar key
#include "../ds/dsSTRID.h"        // ChoosePool/DecideFlyObjectReaction dsVECTOR<dsSTRID> + sret dsSTRID
#include "../m3d/m3dV.h"          // NoticeFlyObject arg / radar geometry

// ws-engine ai08 — a brain's perception module (aiBRAIN::perc pointee). Full DB-verified layout
// (types_members aiPERC / aiPERC_IFACE / aiPERC::INFLICTION / aiPERC::INJURY / aiPERC::INJURY::WOUND).
// The FULL variant reversed by the aiPERC method batch; aiPERC_ZERO is the null variant (next
// frontier).

struct aiBRAIN;              // ai08 — owning brain (ptr; full header included by the .cpp TUs)
struct entENTITY;            // ent subsystem — game entity  boundary (ptr)
struct aiCOORD_GROUP;        // ai08 — coordinated group (ptr; boundary here)
enum AI_PEST : int;          // ai08 — perception-state flag enum (opaque; only packed value read)

// aiPERC_IFACE — thin interface layer: the vtbl base plus the owning-brain back-pointer.
// DB: base aiPERC_BASE@0, pBrain@4 — size 8.
struct aiPERC_IFACE : aiPERC_BASE {
    aiBRAIN *pBrain; // 0x04 owning brain
};

struct aiPERC : aiPERC_IFACE {
    // aiPERC::INFLICTION — accumulated damage this unit has dealt to its current victim
    // (DB size 0xC): dmgAccumulated@0, killed@4, timerLocked@8.
    struct INFLICTION {
        float dmgAccumulated; // 0x00
        int   killed;         // 0x04 number of kills since last report
        float timerLocked;    // 0x08 lock-out countdown before accumulating again
    };

    // aiPERC::INJURY — recent wounds this unit has taken (DB size 0x58).
    struct INJURY {
        // aiPERC::INJURY::WOUND (DB size 8): shooter@0, amount@4.
        struct WOUND {
            ds::WEAK_PTR<entENTITY> shooter; // 0x00 who inflicted it (may have died)
            float                   amount;  // 0x04 damage dealt
        };

        // keyed by the shooter entity pointer -> accumulated wound
        ds::MAP<const void *, WOUND, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> wounds; // 0x00 (0x54)
        float timerDisable; // 0x54 heavy-damage reaction lock-out countdown

        // ?Update@INJURY@aiPERC@@... — decay the tracked wounds by `dt` (scaled by `decayParam`)
        // and advance the reaction lock-out timer. boundary — body external to this batch.
        void Update(float dt, float decayParam);
    };

    dsFLAGS<AI_PEST, int> state;                   // 0x08 perception-state bitflags
    float                 timerClearOnFLOF;        // 0x0C
    float                 timerClearBlockedLOF;    // 0x10
    float                 timerUpdate;             // 0x14
    float                 timerNotifyMovingObject; // 0x18
    float                 timerNotifyGrenade;      // 0x1C grenade-avoidance notify countdown
    int                   grenadeMPID;             // 0x20 uid of the grenade queued for avoidance (-1 = none)
    INFLICTION            infliction;              // 0x24 (0x0C)
    INJURY                injury;                  // 0x30 (0x58, ends 0x88)

    explicit aiPERC(aiBRAIN *pBrain);

    // --- virtual overrides (reversed by this batch) ---
    void ProcessFrame(float dt);
    void ProcessInit();
    bool SetPar(const dsTSTRING<char> &key, float val);
    void NoticeGrenade(entENTITY *entGrenade);
    void NoticeGrenadeTerm(entENTITY *entGrenade);
    void NoticeVictimDamaged(const entENTITY *victim, float amount, bool isKilled);
    void NoticeHeavyDamaged(entENTITY *shooter);
    // ?UpdateRadar@ — virtual; body external to this batch (called via vtbl slot 15). boundary.
    void UpdateRadar(float dt);

    // --- internal (protected) helpers reversed by this batch ---
    void UpdateGrenadeAvoidance();
    bool MaySendEnemyDamaged() const;
    bool ApplyForcedWarPeace();
    void ApplySingleSelectionWarPeace();
    void ApplyGroupSelectionWarPeace(aiCOORD_GROUP &grp);
    void UpdateWarPeace();
    void UpdateVictimDamaged();

    // --- REVERSED in this batch (Saber ai wave, a8_perc.cpp) ---
    // 0x8329E678 (?ChoosePool@aiPERC@@IBAPBV?$dsVECTOR@VdsSTRID@@$07@@M_N@Z) — protected const:
    // pick the fly-object reaction FSM-event pool for a stimulus at distance `dist` on the given
    // side, testing line-of-fire via a collision shoot-cylinder. Null when out of range.
    // REVERSED: src/ws/ai/aiPERC__ChoosePool.cpp.
    const dsVECTOR<dsSTRID, 8> *ChoosePool(float dist, bool onRightSide) const;
    // 0x8329ECA0 (?DecideFlyObjectReaction@aiPERC@@IBA?AVdsSTRID@@ABUm3dV@@@Z) — protected const,
    // sret: choose a random reaction event id for a fly-object seen at `posObject`.
    // REVERSED: src/ws/ai/aiPERC__DecideFlyObjectReaction.cpp.
    dsSTRID DecideFlyObjectReaction(const m3dV &posObject) const;
    // 0x8329F1E0 (?NoticeFlyObject@aiPERC@@UAAXABUm3dV@@@Z) — virtual: a physics object passed near
    // `posObject`; fire the decided reaction FSM event. REVERSED: src/ws/ai/aiPERC__NoticeFlyObject.cpp.
    void NoticeFlyObject(const m3dV &posObject);
    // 0x8329F300 (?UpdateInjury@aiPERC@@IAAXXZ) — protected: scan tracked wounds for a live-shooter
    // heavy hit; if found, clear wounds and raise NoticeHeavyDamaged. REVERSED: src/ws/ai/aiPERC__UpdateInjury.cpp.
    void UpdateInjury();
    // 0x8329F768 (?UpdateTimers@aiPERC@@UAAXM@Z) — virtual: advance all perception timers by `dt`.
    // REVERSED: src/ws/ai/aiPERC__UpdateTimers.cpp.
    void UpdateTimers(float dt);
};
