#pragma once
#include "../m3d/m3dV.h"
#include "../ds/LIST.h"
#include "../ds/WEAK_PTR.h"
#include "../ds/dsSTRID.h"
#include "../ds/dsTSTRING.h"
// ws-engine ai08 "grass" — the grenade/rocket awareness sub-system embedded in aiPLANNER
// (aiPLANNER.grass). DB-verified layout (types_members aiGRASS) — size 0x40. The grenade
// tracking lists (grenInits/grenades) are now typed against their DB element layouts
// (aiGRASS::GRENADE_INFO / aiGRASS::GRENADE_INIT_DATA).

struct entENTITY;      // ent subsystem — entity                                boundary (ptr)
struct aiBRAIN;        // ai08 — brain (aiBRAIN.h)                               boundary (ptr)
struct aiWATCHER;      // ai08 — watcher (full definition aiWATCHER.h)           (ptr)
struct pjlPJL;         // pjl subsystem — projectile logic instance              boundary (ptr)
struct aiCOORD_GROUP;  // ai08 — coordinated squad/group                         boundary (ptr)
struct navWP;          // nav subsystem — waypoint                               boundary (ptr)

struct aiGRASS {
    // DB types_members aiGRASS::GRENADE_INFO — a tracked in-flight grenade (size 0x30).
    struct GRENADE_INFO {
        ds::WEAK_PTR<entENTITY> shooter;       // 0x00 weak ref to the thrower
        const dsSTRID           shooterTeamID; // 0x04 thrower's team id at throw time
        const entENTITY        *grenade;       // 0x08 the grenade entity
        const m3dV              posStart;      // 0x0C centre at throw time
        const m3dV              posEnd;        // 0x18 predicted landing/target point
        m3dV                    posLastKnown;  // 0x24 last observed centre

        // 0x831848C8 — construct from thrower entity, grenade entity, start & end positions.
        GRENADE_INFO(const entENTITY *shooter, const entENTITY *grenade,
                     const m3dV &posStart, const m3dV &posEnd);
        // 0x83183B90 — copy constructor.
        GRENADE_INFO(const GRENADE_INFO &other);
        // 0x83183C38 — destructor (releases the weak ref).
        ~GRENADE_INFO();
    };

    // DB types_members aiGRASS::GRENADE_INIT_DATA — a grenade awaiting registration (size 0x18).
    struct GRENADE_INIT_DATA {
        const entENTITY *grenade;    // 0x00
        const entENTITY *shooter;    // 0x04
        float            timerDelay; // 0x08
        m3dV             accel;      // 0x0C
    };

    int                        cntFramesRocket; // 0x00 frames remaining of rocket-awareness boost
    ds::LIST<GRENADE_INIT_DATA> grenInits;      // 0x04 grenades queued for delayed init
    ds::LIST<GRENADE_INFO>      grenades;        // 0x18 grenades currently tracked in flight
    ds::LIST<aiWATCHER*>        victims;         // 0x2C watchers flushed out of hideouts (smoke-out)

    // --- reversed in prior batches ---
    void NoticeFlyingRocket(pjlPJL *rocket, entENTITY *shooter);
    aiWATCHER *GetVictim(aiCOORD_GROUP *group) const;
    void ProcessSmokingOut();

    // 0x83183748 (?GetGrenadeInfo@aiGRASS@@QBAPBUGRENADE_INFO@1@H@Z) — the tracked in-flight grenade
    // whose instance id is `grenadeMPID`, or null when none is tracked.  boundary.
    const GRENADE_INFO *GetGrenadeInfo(int grenadeMPID) const;

    // --- reversed in this batch ---
    // 0x83184A10 — append a freshly-constructed GRENADE_INFO to `grenades`.
    void AddFlyingGrenade(const entENTITY *shooter, const entENTITY *grenade,
                          const m3dV &posStart, const m3dV &posEnd);
    // 0x83184A80 — remove every tracked grenade whose entity is `grenade`.
    void RemoveFlyingGrenade(const entENTITY *grenade);
    // 0x83184D78 — per-frame tick: prune smoke-outs, drain grenInits, notify NPCs, decay timers.
    void ProcessFrame(float dt);
    // 0x83184E10 — a member of `group` was smoked out of a hideout: create/track its watcher and
    // clear weapon-strike sparsing for every (weapon, team) pair the group can bring to bear.
    void SmokeOutFromHO(aiCOORD_GROUP &group);
    // 0x831850D0 (?NoticeGrenadeInit@aiGRASS@@QAAXPBVentENTITY@@0ABUm3dV@@1@Z) — register a grenade
    // thrown by `shooter` with explicit start & end positions; when the shooter is-a aiBRAIN_DESC,
    // also kick off hideout smoke-out handling.
    void NoticeGrenadeInit(const entENTITY *shooter, const entENTITY *grenade,
                           const m3dV &posStart, const m3dV &posEnd);
    // 0x83185188 (?NoticeGrenadeInit@aiGRASS@@QAAXPBVentENTITY@@0ABUm3dV@@@Z) — register a grenade by
    // acceleration: for an aiBRAIN shooter resolve start (grenade centre) & end (shooter status pos)
    // and delegate to the explicit-position overload, otherwise queue it for delayed init.
    void NoticeGrenadeInit(const entENTITY *shooter, const entENTITY *grenade, const m3dV &accel);

    // --- reversed in this batch ---
    // 0x83183518 — for each tracked grenade, refresh its last-known centre and notify every enemy
    // unit-watcher within its avoid radius.
    void SendNotificationsToNPC();
    // 0x831834B8 — decay the delay timer of every queued grenInits entry by `dt`.
    void UpdateTimers(float dt);
    // 0x83183AB8 — if the brain's enemy peeker is a tracked victim, destroy and unlink it.
    void HandleSmokingOut(const aiBRAIN *shooter);
    // 0x83183818 — queue a grenade (given by acceleration) for delayed init on the grenInits list.
    void AddFlyingGrenadeForInit(const entENTITY *shooter, const entENTITY *grenade,
                                 const m3dV &accel);
    // 0x83183650 — path-build penalty contribution from currently tracked grenades near `wp`.
    float CalcPenaltyForBuildPath(navWP *wp) const;
    // 0x83183780 — tell the brain's perception module about every tracked grenade.
    void SendGrenadesDanger(aiBRAIN *pBrain) const;
    // 0x83183980 — may this brain throw a grenade at its current enemy peeker right now?
    bool CanThrowGrenade(const aiBRAIN *pBrain) const;
    // 0x83183F80 — append a human-readable dump of the grenades/victims lists to `line`.
    void DbgGrenadesLine(dsTSTRING<char> &line);
    // 0x831849A8 — clear all three lists (destroying victim watchers).
    void DropAll();

    // --- boundary (bodies external to this batch) ---
    void NoticeGrenadeTerm(const entENTITY *grenade);
};
