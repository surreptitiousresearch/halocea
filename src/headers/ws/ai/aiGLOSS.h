#pragma once
#include "../ds/dsVECTOR.h"
#include "../ds/dsSTRID.h"
#include "../ds/dsTSTRING.h"

// Forward decls for method parameter types (used only by pointer/ref in the decls below).
struct wpnS_DESC;    // wpn/wpnS_DESC.h
struct aiS_VARS;     // ai — strike-vars snapshot (opaque here; only passed by const ptr)
struct aiBRAIN;      // ai/aiBRAIN.h

// ws-engine ai: per-actor "gloss" bookkeeping for weapon strikes — tracks which strike sounds
// are temporarily disabled and the per-SUID/per-team "sparsing" timers that throttle how often a
// strike may play. DB-verified layout (types_members aiGLOSS): sparsings@0, strikeDisabled@20.
struct aiGLOSS {
    // One throttling record for a (weapon-SUID, shooter-team) pair. DB-verified layout
    // (types_members aiGLOSS::SparsingData): SUID@0, teamShooter@4, isPlaying@8, timer@12 — size 16.
    // DEVIATION: DB marks SUID/teamShooter `const`; dropped here so FindCreateSparsing can populate a
    // freshly-constructed record by field assignment (const-ness is source-only; layout unaffected).
    struct SparsingData {
        int     SUID;        // 0x00 weapon strike unique id
        dsSTRID teamShooter; // 0x04 interned shooter-team id
        bool    isPlaying;   // 0x08 a strike of this kind is currently sounding
        float   timer;       // 0x0C cooldown remaining before the strike may play again
    };

    dsVECTOR<SparsingData, 8>       sparsings;      // 0x00 active per-(SUID,team) throttle records
    dsVECTOR<dsTSTRING<char>, 8>    strikeDisabled; // 0x14 strike-sound names currently disabled

    // 0x8323A4D0 / 0x8323A5A8 — locate the record matching (SUID, teamShooter); null if none.
    // teamShooter is a dsSTRID passed by value (mangling VdsSTRID@@ — a single interned-id pointer).
    const SparsingData *FindSparsing(int SUID, dsSTRID teamShooter) const;
    SparsingData       *FindSparsing(int SUID, dsSTRID teamShooter);
    // 0x8323AA30 — FindSparsing, appending a fresh zeroed record on a miss.
    SparsingData       *FindCreateSparsing(int SUID, dsSTRID teamShooter);

    // 0x8323A760 — true when `desc`'s strike sound is NOT in strikeDisabled.
    bool IsStrikeEnabled(const wpnS_DESC &desc) const;
    // 0x8323A800 — true when the strike may fire now (not SUID-sparsed, or its timer has elapsed).
    bool IsStrikeAvailable(const wpnS_DESC &desc, dsSTRID teamShooter) const;
    // 0x8323A8D0 — remaining sparse cooldown for (SUID, teamShooter); 0 if no record.
    float GetSparseTimer(int SUID, dsSTRID teamShooter) const;

    // 0x8323AAB8 — mark the (SUID, shooter-team) record as currently playing.
    void NoticeStrikeStart(const aiS_VARS &vars, const wpnS_DESC &desc, aiBRAIN *shooter);
    // 0x8323AB20 — add/remove `name` from the disabled-strikes list.
    void EnableStrike(const dsTSTRING<char> &name, bool isEnable);
    // 0x8323ABA8 — set the (SUID, teamShooter) cooldown timer and clear its playing flag.
    void SetSparsing(const wpnS_DESC &desc, dsSTRID teamShooter, float time);
    // ?ClearSparsing@aiGLOSS@@QAAXABUwpnS_DESC@@VdsSTRID@@@Z — drop any sparsing reservation the
    // (weapon-descriptor, team) pair holds. boundary (was a local shim in aiGRASS__SmokeOutFromHO.cpp).
    void ClearSparsing(const wpnS_DESC &desc, dsSTRID team);

    // ?ProcessFrame@aiGLOSS@@QAAXM@Z @ 0x8323A3E8 — per-frame tick: age every sparsing timer by
    // `delta` (skipped entirely while AI is off and no watchee is moving).
    void ProcessFrame(float delta);
    // 0x8323A070 (?DropAll@aiGLOSS@@QAAXXZ) — clear all sparsing state. boundary.
    void DropAll();
};
