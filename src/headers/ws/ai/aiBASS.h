#pragma once
#include "../ds/dsTSTRING.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsSTRID.h"
#include "../ds/WEAK_PTR.h"
#include "../ds/MAP.h"
#include "../ssl/sslOBJ_REF.h"

// ws-engine ai08 — aiBASS ("battle sound system"): plays situational exclamation sounds for AI.
// DB-verified layout (types_members aiBASS), size 148.

struct aiBASS_vtbl; // ai08 — aiBASS vtable boundary
struct aiBRAIN;     // ai/aiBRAIN.h — exStream is keyed by aiBRAIN* (pointer only here)
struct psSECTION;           // ws/ps — section handle (ReadExclamation/ParsePS operate on one)
struct iaIACTOR;     // ia/iaIACTOR.h — full definition supplied by aiBASS .cpp translation units
struct sslCLASS_REF; // ssl/sslCLASS_REF.h — full definition supplied by aiBASS .cpp translation units
struct dsDATA;       // ds/dsDATA.h — full definition supplied by aiBASS .cpp translation units

struct aiBASS {
    // How a new exclamation overlaps one already playing. DB-verified enum, size 4.
    enum exOVERLAP_RES {
        exOVERLAP_INVALID = -1,
        exOVERLAP_SKIP    = 0,
        exOVERLAP_DELAY   = 1,
        exOVERLAP_REPLACE = 2,
    };

    // One configured exclamation category. DB-verified layout (types_members aiBASS::exDATA),
    // size 28.
    struct exDATA {
        dsSTRID       name;            // 0x00 exclamation id
        float         priority;        // 0x04
        float         aiCoolDown;      // 0x08
        float         exCoolDown;      // 0x0C
        int           exMaxCount;      // 0x10
        bool          isAcceptForDead; // 0x14 may play for a dead speaker
        exOVERLAP_RES overlap;         // 0x18 overlap policy
    };

    // One live, in-flight exclamation being tracked in the per-brain stream. DB-verified layout
    // (types_members aiBASS::exSTREAM_DATA), size 44.
    struct exSTREAM_DATA {
        dsSTRID               name;              // 0x00 exclamation id
        dsSTRID               nameSnd;           // 0x04 resolved sound id
        ds::WEAK_PTR<aiBRAIN> owner;             // 0x08 speaking brain (weak)
        float                 priority;          // 0x0C
        float                 totalTime;         // 0x10
        float                 time;              // 0x14 elapsed since start
        float                 aiCoolDown;        // 0x18
        float                 exCoolDown;        // 0x1C
        float                 dist;              // 0x20 distance to closest player at play time
        exOVERLAP_RES         overlap;           // 0x24
        bool                  isFinishEventSent; // 0x28

        // 0x832403D8 (?_IsExpired@) — reached via the public IsExpired wrapper. boundary.
        bool IsExpired() const;
        // 0x83240438 (?_IsExExpired@) — reached via the public IsExExpired wrapper. boundary.
        bool IsExExpired() const;
        // 0x83240378 (?IsStarted@) — true once the sound has begun playing. boundary.
        bool IsStarted() const;
        // exSTREAM_DATA::IsJustFinished — the finish event fired this frame. boundary.
        bool IsJustFinished() const;
        // exSTREAM_DATA::IsAIExpired — the owning brain has died / is gone. boundary.
        bool IsAIExpired() const;
        // exSTREAM_DATA::Start — kick off playback; returns false if it could not start. boundary.
        bool Start();

        // ctor from a configured category, the resolved sound id, the owning brain and the play
        // distance. boundary.
        exSTREAM_DATA(const exDATA *exclamation, const dsSTRID *soundID, aiBRAIN *owner, float dist);
        ~exSTREAM_DATA(); // boundary
    };

    typedef ds::MAP<aiBRAIN *, exSTREAM_DATA, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> ExStreamMap;
    typedef ds::impl::MAP_ITERATOR<aiBRAIN *, exSTREAM_DATA, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> ExStreamIter;
    typedef ds::impl::MAP_CONST_ITERATOR<aiBRAIN *, exSTREAM_DATA, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> ExStreamConstIter;

    aiBASS_vtbl          *__vftable;          // 0x00
    dsTSTRING<char>       exclamationPrefix;  // 0x04
    dsTSTRING<char>       exclamationPostfix; // 0x08
    float                 aiCoolDown;         // 0x0C
    float                 exCoolDown;         // 0x10
    float                 minSimSoundDelay;   // 0x14
    float                 maxDistance;        // 0x18
    float                 distanceDelta;      // 0x1C
    int                   maxSounds;          // 0x20
    dsVECTOR<exDATA, 8>   exclamations;       // 0x24
    ExStreamMap           exStream;           // 0x38 per-brain in-flight exclamation stream (84B)
    sslOBJ_REF            sslObj;             // 0x8C scripted-sound object ref
    dsTSTRING<char>       sslObjName;         // 0x90

    // 0x83243970 (?FindExclamation@aiBASS@@MAAABVexDATA@1@VdsSTRID@@@Z) — look up the configured
    // exclamation whose name matches `id`. boundary.
    const exDATA &FindExclamation(dsSTRID id);

    // 0x83240460 (?IsExAcceptForDead@aiBASS@@UAA_NABVdsSTRID@@@Z) — true when exclamation `exID` is
    // configured (valid name, non-negative priority, a real overlap policy) and may play for a dead
    // speaker. REVERSED.
    bool IsExAcceptForDead(const dsSTRID &exID);

    // 0x83240500 (?Term@aiBASS@@MAAXXZ) — tear down: remove the scripted-sound object from the ssl
    // system if one was created. REVERSED.
    void Term();

    // 0x83243DF8 (?GetExFromStream@...ABVdsSTRID@@@Z) — first non-expired stream entry whose name
    // matches `soundID`, else null. REVERSED.
    const exSTREAM_DATA *GetExFromStream(const dsSTRID &soundID) const;
    // 0x83243F30 (?GetExFromStream@...M@Z) — first non-expired stream entry whose elapsed time is
    // <= `maxTimeFromStart`, else null. REVERSED.
    const exSTREAM_DATA *GetExFromStream(float maxTimeFromStart) const;
    // 0x83244070 (?GetExCountInStream@...ABVdsSTRID@@@Z) — count of non-expired stream entries whose
    // name matches `soundID`. REVERSED.
    int GetExCountInStream(const dsSTRID &soundID) const;
    // 0x832441A0 (?TryToReplace@...ABVexDATA@1@M@Z) — pick the best-priority stream entry to
    // displace for `exclamation` at `dist`, then defer to the 4-arg TryToReplace helper. REVERSED.
    bool TryToReplace(const exDATA &exclamation, float dist);
    // 0x83244868 (?ReadExclamation@aiBASS@@IAAXABVpsSECTION@@PBD@Z) — parse one exclamation category
    // named `name` from section `psRead` and append it to `exclamations`. REVERSED.
    void ReadExclamation(const psSECTION &psRead, const char *name);
    // 0x83244FF8 (?ParsePS@aiBASS@@MAA_NVpsSECTION@@AAV?$dsTSTRING@D@@@Z) — read global BASS tuning
    // from section `psRead` (prefixes, cooldowns, distances). `psRead` is taken by value (a psSECTION
    // handle). REVERSED.
    bool ParsePS(psSECTION psRead, dsTSTRING<char> &hintErr);
    // 0x832453C8 (?ProcessStream@...M@Z) — per-frame stream tick: start pending sounds, fire
    // finish/cancel events, reap expired entries, advance timers. REVERSED.
    void ProcessStream(float dt);
    // 0x83245760 (?RemoveExFromStream@...PBVexSTREAM_DATA@1@@Z) — mute and erase the stream entry
    // for `ex`'s owning brain. REVERSED.
    void RemoveExFromStream(const exSTREAM_DATA *ex);
    // 0x83245EF0 (?InternalPlayEx@...PAVaiBRAIN@@ABVexDATA@1@@Z) — validate and enqueue an
    // exclamation for `pBrain`; returns whether it was accepted. REVERSED.
    bool InternalPlayEx(aiBRAIN *pBrain, const exDATA &exclamation);

    // Helpers reached from the above (bodies are the next frontier). boundary.
    bool IsBetterPriorDist(float priorityA, float priorityB, float distA, float distB, float eps) const;
    bool TryToReplace(const exSTREAM_DATA *victim, const exDATA &exclamation, float dist); // 4-arg helper (0x83240810)
    // 0x83243180 (?InternalMuteSound@aiBASS@@MAA_NPBVexSTREAM_DATA@1@@Z) — mute `ex`'s backing sound
    // on its owning brain and fire the "cancelled" event. REVERSED.
    virtual bool InternalMuteSound(const exSTREAM_DATA *ex);
    // 0x83242F88 (?InternalPlaySound@aiBASS@@MAA_NPBVexSTREAM_DATA@1@AAM@Z) — start `ex`'s backing
    // sound on its owning brain; writes the resolved duration into `outTime`. REVERSED.
    virtual bool InternalPlaySound(const exSTREAM_DATA *ex, float &outTime);
    // 0x83245BC8 (?IsStreamFull@aiBASS@@MBA_NXZ) — true when the in-flight stream has reached its
    // capacity (maxSounds). boundary. Declared non-virtual to preserve the DB-verified size 148.
    bool IsStreamFull() const;
    bool FilterExVsStream(aiBRAIN *pBrain, const exDATA &exclamation, float dist);
    exOVERLAP_RES ParseOverlap(const dsTSTRING<char> *text, exOVERLAP_RES fallback);

    // 0x83241CB8 (?ProcessFrame@aiBASS@@UAAXM@Z) — per-frame tick (dispatched through aiBASS's own
    // manual __vftable; declared non-virtual here to keep the DB-verified size 148). boundary.
    void ProcessFrame(float delta);
    // 0x83241928 (?InitLevel@aiBASS@@UAAXXZ) — per-level init (see note above). boundary.
    void InitLevel();

    // 0x83242AE8 (?Init@aiBASS@@MAA_NXZ) — load bass.cls, parse the PS config, register the SSL
    // class + native func callbacks, then create and name the scripted-sound object. REVERSED.
    bool Init();

    // 0x83243D68 (?GetExFromStream@aiBASS@@MBAPBVexSTREAM_DATA@1@PAVaiBRAIN@@@Z) — the live,
    // non-expired stream record owned by `brain`, or null when absent/expired. REVERSED.
    const exSTREAM_DATA *GetExFromStream(aiBRAIN *brain) const;

    // 0x83242810 (?GetCurrentEx@aiBASS@@UAAABV?$dsTSTRING@D@@PAViaIACTOR@@@Z) — name of the
    // exclamation `actor` is currently speaking (empty when none / not an aiBRAIN). REVERSED.
    const dsTSTRING<char> &GetCurrentEx(iaIACTOR *actor);
    // 0x83245BB8 (?GetSslObjName@aiBASS@@UBAABV?$dsTSTRING@D@@XZ) — the scripted-sound object's
    // name, for diagnostics. boundary.
    const dsTSTRING<char> &GetSslObjName() const;

    // 0x832412C8 (?PlayExclamation@aiBASS@@UAA_N...@Z) — queue/play `sound` for `actor`. boundary.
    bool PlayExclamation(iaIACTOR *actor, const dsTSTRING<char> &sound, float priority,
                         const dsTSTRING<char> &overlap);
    // 0x83241140 (?PlayExclamationInstant@aiBASS@@UAA_N...@Z) — play `sound` for `actor` now.
    // boundary.
    bool PlayExclamationInstant(iaIACTOR *actor, const dsTSTRING<char> &sound);

    // 0x83242618 (?RegisterSslFuncCb@aiBASS@@SA_NAAVsslCLASS_REF@@@Z) — register the three native
    // script callbacks (PlayExclamation, PlayExclamationInstant, GetCurrentEx) on `sslClass`.
    // REVERSED.
    static bool RegisterSslFuncCb(sslCLASS_REF &sslClass);

    // SSL member-callback trampolines (sslCB_MEMBER_FN shape). REVERSED.
    // 0x832424B0 (?cbPlayExclamation@aiBASS@@QAAX...)
    void cbPlayExclamation(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x83242398 (?cbPlayExclamationInstant@aiBASS@@QAAX...)
    void cbPlayExclamationInstant(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x832417F8 (?cbGetCurrentEx@aiBASS@@QAAX...)
    void cbGetCurrentEx(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);

    // 0x83241560 (?_SSL_V_V@aiBASS@@IAAXABVdsSTRID@@@Z) — invoke the scripted-sound object's
    // `funcName` void()->void handler. REVERSED.
    void _SSL_V_V(const dsSTRID &funcName);
    // 0x832419A0 (?_SSL_V_F@aiBASS@@IAAXABVdsSTRID@@M@Z) — invoke the scripted-sound object's
    // `funcName` void(float) handler. REVERSED.
    void _SSL_V_F(const dsSTRID &funcName, float arg1);
};
