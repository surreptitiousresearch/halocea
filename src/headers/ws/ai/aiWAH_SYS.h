#pragma once
#include "../ds/dsVECTOR.h"

// ws-engine ai08 "watcher" subsystem — tracks active AI units/players/entities.
// DB-verified layout (types_members aiWAH_SYS) — size 80 (0x50): four parallel watcher lists.

struct aiWTC_HOLDER;      // aiWTC_HOLDER.h
struct aiWATCHER;         // aiWATCHER.h
struct aiWATCHER_BRAIN;   // aiWATCHER_BRAIN.h — unit watcher
struct aiWATCHER_ENT;     // aiWATCHER_ENT.h — entity watcher
struct aiBRAIN;           // ai08 — per-unit AI brain           boundary (fwd)
struct aiFEE;             // aiFEE.h
struct iaIACTOR;          // ia subsystem — interactive actor   boundary (fwd)

struct aiWAH_SYS {
    dsVECTOR<aiWTC_HOLDER *, 8>    storage;    // 0x00
    dsVECTOR<aiWATCHER *, 8>       arrPlayers; // 0x14 active player watchers
    dsVECTOR<aiWATCHER_BRAIN *, 8> arrUnits;   // 0x28 active AI-unit watchers
    dsVECTOR<aiWATCHER_ENT *, 8>   arrEnts;    // 0x3C active entity watchers

    // Optional predicate selecting which watchers PutAllFEEs emits. size 4 (vtable only).
    struct IsIncludeFEECheck {
        void *__vftable;                                   // 0x00 IsIncludeFEECheck_vtbl*
        // vtbl+0x4 — include the FEE for `watcher`?
        bool operator()(aiWATCHER *const watcher);
    };

    // 0x83172048 — get (or create + push) the holder watching `actor`, bumping its counter.
    aiWTC_HOLDER *Demand(iaIACTOR *actor);
    // 0x83172128 / 0x83172190 — drop the holder for a watcher / actor.
    void Forgo(aiWATCHER *watcher);
    void Forgo(iaIACTOR *actor);
    // Private index-based drop (ai08). boundary — deeper than this wave.
    void Forgo(aiWTC_HOLDER *holder, int idx);
    // 0x83172218 — rebuild arrPlayers from the current player set; false if no players.
    bool TryGettingPlayers();
    // 0x831722D8 — drop all players/ents/units and clamp every holder counter to <=1.
    void DropAll();
    // 0x83172540 — register `wh`'s watcher as an active unit and wire its brain's peekerSelf.
    void RegisterActive(aiWTC_HOLDER *wh);
    // 0x83172630 — remove the active-unit watcher tracking `pBrain` and forgo it.
    void UnRegisterActive(aiBRAIN *pBrain);
    // 0x831728B0 — append an aiFEE for every valid holder passing `checker` (null = all).
    void PutAllFEEs(dsVECTOR<aiFEE, 8> &arr, IsIncludeFEECheck *checker);
    // 0x83172968 — ensure an entity watcher exists for `actor` (created foe-eligible).
    void RegisterActiveEnt(iaIACTOR *actor);

    // 0x83171910 — the idx-th active unit watcher (null when idx is out of range).
    aiWATCHER_BRAIN *PeekActive(int idx) const;

    // 0x831718C0 — the idx-th active player watcher (null when idx is out of range).
    aiWATCHER *PeekPlayer(int idx) const;

    // 0x83170678 — count of active unit brains currently registered in the message system that
    // report a non-zero priority-int (i.e. actively participating).
    int GetAllUnitsCnt() const;

    // 0x83171740 — per-frame tick: Update(time) every holder's watcher.
    void ProcessFrame(float time);

    // 0x83171848 — the active watcher whose UID equals `uid` (null if none is registered).
    aiWATCHER *PeekByUID(unsigned int uid) const;

    // 0x83171960 — remove the entity watcher tracking `actor` (and clear its foe flag); also
    // clears the foe flag on any holder-side entity watcher still pointing at `actor`.
    void UnRegisterActiveEnt(iaIACTOR *actor);

    // 0x83171BA8 — true if any active-unit watcher tracks `pBrain`; also compacts out duplicate
    // stale entries it finds for that brain.
    bool IsRegistered(aiBRAIN *pBrain);
};
