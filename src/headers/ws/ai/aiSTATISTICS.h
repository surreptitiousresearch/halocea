#pragma once
// ws-engine ai08 — the planner's runtime statistics block (aiPLANNER::stat). Only the method the
// batched aiMIND methods reach is declared; the full layout/bodies are the next frontier.  boundary.
// DB-verified size 84 (0x54); layout reconciled to the DB member list 2026-08-04.

#include "../ds/dsSTRID.h"
#include "../ds/MAP.h"
struct aiBRAIN; // ai08 — per-unit AI brain   boundary (fwd)

// DB-verified layout (types_members aiSTATISTICS, size 0x54): the whole block is the single
// member cntTeamUnits — a chained-hash team->unit-count map (ds::MAP<dsSTRID,int,...>, itself
// size 0x54). The map-mutating methods operate on it directly (the disassembly calls
// ds::MAP::Find/Begin/FindInsert with `this` as the map). See the .cpp files.
typedef ds::MAP<dsSTRID, int, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> aiSTATISTICS_TEAM_MAP;

struct aiSTATISTICS {
    aiSTATISTICS_TEAM_MAP cntTeamUnits; // 0x00 live-unit count per team (DB: cntTeamUnits)

    // 0x832AA418 (?NoticeUnitCreated@aiSTATISTICS@@QAAXPBVaiBRAIN@@@Z) — record that a new unit was
    // created. boundary — body external to this batch.
    void NoticeUnitCreated(const aiBRAIN *pBrain);
    // 0x832AA... (?NoticeUnitRemoved@aiSTATISTICS@@QAAXPBVaiBRAIN@@@Z) — record that a unit was
    // removed. boundary — body external to this batch.
    void NoticeUnitRemoved(const aiBRAIN *pBrain);
    // 0x832A9EC8 (?ProcessFrame@aiSTATISTICS@@QAAXM@Z) — per-frame stats tick. boundary.
    void ProcessFrame(float delta);
    // ?GetActiveUnitsCnt@aiSTATISTICS@@QBAHABVdsSTRID@@@Z — count active units on team `team`.
    // boundary — body external to this batch.
    int GetActiveUnitsCnt(const dsSTRID &team) const;

    // --- methods reversed in the saber2 drain (a8_statistics.cpp) ---
    // 0x832A9E08 (?UpdatePlayerDirectlySeen@aiSTATISTICS@@IAAXXZ) — set/clear the planner's
    // "player directly seen" state bit (0x10000) by scanning every active unit watcher.
    // REVERSED: src/ws/ai/aiSTATISTICS__UpdatePlayerDirectlySeen.cpp.
    void UpdatePlayerDirectlySeen();
    // 0x832A9F10 (?SumInitedActiveUnits@aiSTATISTICS@@IBAHXZ) — count active-unit brains whose
    // status "un-inited" gen bit (0x20) is clear. REVERSED: aiSTATISTICS__SumInitedActiveUnits.cpp.
    int SumInitedActiveUnits() const;
    // 0x832AA158 (?SumTeamUnits@aiSTATISTICS@@IBAHXZ) — sum every team's unit count in the map.
    // REVERSED: src/ws/ai/aiSTATISTICS__SumTeamUnits.cpp.
    int SumTeamUnits() const;
    // 0x832AA298 (?NoticeTeamChanged@aiSTATISTICS@@IAAXPBVaiBRAIN@@ABVdsSTRID@@1@Z) — decrement the
    // old team's live-unit count and increment the new team's (both clamped >= 0), logging when a
    // team is not gs-registered. REVERSED: src/ws/ai/aiSTATISTICS__NoticeTeamChanged.cpp.
    void NoticeTeamChanged(const aiBRAIN *brain, const dsSTRID &oldTeam, const dsSTRID &newTeam);
    // 0x832A9F98 (?GetActiveUnitsCnt@aiSTATISTICS@@QAAHVdsSTRID@@@Z) — by-value/non-const overload:
    // the recorded live-unit count for team `team` (0 when absent).
    // REVERSED: src/ws/ai/aiSTATISTICS__GetActiveUnitsCnt.cpp.
    int GetActiveUnitsCnt(dsSTRID team);
};
