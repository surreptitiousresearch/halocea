#pragma once
// HCEX sound bridge: the top-level Halo audio manager. Owns the FMOD system handle, the
// per-level FSB sound-list table, the virtual-channel pool, tag-name playback parameter
// tables, and the debug custom-sound list. Derives from snd::SYSTEM_CUSTOM (the low-level
// snd:: driver's abstract backend interface) so the outer ws-engine snd:: layer can drive it
// through a uniform vtable (Init/Term/Update/InitLevel/TermLevel/DbgRenderSounds).
//
// DB-verified layout (types_members HALO_SOUND_SYSTEM) — size 376:
//   [base snd::SYSTEM_CUSTOM]@0 (vtable ptr, 4 bytes)
//   system@4 (FMOD::System*), minDelay@8, outputRate@12, isLevelInited@16, isPaused@17,
//   isCinematicMode@18, pauseTime@20 (UINT64_T), nextMouthSoundPlay@28, dbgLock@32 (osLOCK,
//   52 bytes), soundLists@84 (dsVECTOR<HALO_SOUND_LIST,8>), preloadList@104
//   (dsVECTOR<HALO_PERM_SOUND,8>), channels@124 (dsVECTOR<HALO_CHANNEL,8>), customSounds@144
//   (dsVECTOR<HALO_CUSTOM_SOUND,8>), params@164 (dsVECTOR<HALO_SOUND_PARAMS,8>),
//   paramsWildcard@184 (dsVECTOR<HALO_SOUND_PARAMS,8>), playToEnd@204
//   (dsVECTOR<HALO_CHANNEL,8>), tagInfoPool@224 (ds::NODE_POOL<TAG_INFO,TAG_INFO_POLICY>, 40
//   bytes), listenerProps@264 (HALO_SOUND_SYSTEM::<unnamed_type_listenerProps>, 112 bytes).

#include "../ws/snd/snd_fmod_boundary.h"
#include "../ws/os/osLOCK.h"
#include "../ws/ds/dsVECTOR.h"
#include "../UINT64_T.h"
#include "../ws/m3d/m3dV.h"
#include "../ws/m3d/m3dMATR.h"
#include "HALO_SOUND_LIST.h"
#include "HALO_PERM_SOUND.h"
#include "HALO_CHANNEL.h"
#include "HALO_CUSTOM_SOUND.h"
#include "HALO_SOUND_PARAMS.h"
#include "HALO_CHANNEL_PROPERTIES.h"
#include "TAG_INFO.h"

struct sound_permutation; // full layout in ../sound_permutation.h
struct HALO_SOUND_SYSTEM_vtbl; // boundary — inherited from snd::SYSTEM_CUSTOM_vtbl, overridden slots below

struct HALO_SOUND_SYSTEM : snd::SYSTEM_CUSTOM {
    // The anonymous inner class the mangler calls `<unnamed_type_listenerProps>` — snapshot of
    // the audio listener (camera) transform used for 3D panning math. DB-verified layout
    // (types_members HALO_SOUND_SYSTEM::<unnamed_type_listenerProps>): pos@0 (m3dV), vel@12
    // (m3dV), forw@24 (m3dV), up@36 (m3dV), matr@48 (m3dMATR, 64 bytes) — size 112.
    struct listenerProps_t {
        m3dV     pos;   // 0x00
        m3dV     vel;   // 0x0C
        m3dV     forw;  // 0x18
        m3dV     up;    // 0x24
        m3dMATR  matr;  // 0x30

        // 0x836B5580 — default ctor; every field is left default-constructed (m3dV/m3dMATR have
        // no member ctors of their own reversed here), so the generated body is empty.
        listenerProps_t();
    };

    // __vftable lives at offset 0x00, inherited from snd::SYSTEM_CUSTOM — not redeclared here.

    FMOD::System              *system;               // 0x04
    unsigned int                minDelay;              // 0x08 FMOD DSP buffer length, doubled
    int                          outputRate;             // 0x0C FMOD software output sample rate
    bool                         isLevelInited;           // 0x10
    bool                         isPaused;                 // 0x11
    bool                         isCinematicMode;           // 0x12
    unsigned char                _pad13;                    // 0x13
    UINT64_T                    pauseTime;                  // 0x14 DSP clock at the moment Pause(true) ran
    int                          nextMouthSoundPlay;          // 0x1C osGetTime() throttle for mouth/skull VO
    osLOCK                      dbgLock;                       // 0x20 guards Update()/DbgRenderSounds() overlap
    dsVECTOR<HALO_SOUND_LIST, 8>     soundLists;                // 0x54 fixed 15-entry FSB bank table
    dsVECTOR<HALO_PERM_SOUND, 8>     preloadList;                // 0x68 preloaded-but-not-yet-playing sounds
    dsVECTOR<HALO_CHANNEL, 8>         channels;                    // 0x7C live virtual channels
    dsVECTOR<HALO_CUSTOM_SOUND, 8>    customSounds;                 // 0x90 one-off filename-based sounds
    dsVECTOR<HALO_SOUND_PARAMS, 8>    params;                        // 0xA4 tag-name params, exact match
    dsVECTOR<HALO_SOUND_PARAMS, 8>    paramsWildcard;                 // 0xB8 tag-name params, wildcard match
    dsVECTOR<HALO_CHANNEL, 8>          playToEnd;                       // 0xCC channels stopping "play to end"
    ds::NODE_POOL<TAG_INFO, TAG_INFO_POLICY>  tagInfoPool;                // 0xE0 per-tag play-count pool
    listenerProps_t                     listenerProps;                     // 0x108 last-set 3D listener transform

    static HALO_SOUND_SYSTEM_vtbl vftable; // `HALO_SOUND_SYSTEM::`vftable'' — boundary

    HALO_SOUND_SYSTEM();  // 0x836C1CB0 — registers the singleton (haloSoundSystem)
    ~HALO_SOUND_SYSTEM(); // 0x836C0BB4

    // 0x836C1EA4 — compiler-generated scalar deleting destructor (`??_G` mangle).
    HALO_SOUND_SYSTEM *scalarDeletingDtor(unsigned char deleteFlags);

    // ---- snd::SYSTEM_CUSTOM overrides ----
    bool Init(snd::INIT init);         // 0x836C523C
    void Term();                        // 0x836BD7C8
    void Update();                      // 0x836C4388
    void InitLevel(const dsTSTRING<char> &fullName); // 0x836C03F0
    void TermLevel();                    // 0x836C3988
    void DbgRenderSounds();               // 0x836BD890

    // ---- public API ----
    void SetCinematicMode(bool enable);                 // 0x836B5188
    bool PreloadSound(const sound_permutation *perm, bool load); // 0x836B5804 — stub (always true)
    void Pause(bool pause);                              // 0x836BCC44
    // DEVIATION: the funcs.prototype convenience string types channelProps as
    // `const HALO_CHANNEL_PROPERTIES *`, but the mangled symbol itself
    // (?FindSoundByName@HALO_SOUND_SYSTEM@@QAA_NPBUsound_permutation@@JABUHALO_CHANNEL_PROPERTIES@@...)
    // decodes the 3rd argument as `ABU` = reference-to-const, i.e. `const HALO_CHANNEL_PROPERTIES &`.
    // Per project convention the mangled name is ground truth over the decompiler's convenience
    // prototype string, so this stays reference-typed (HALO_PERM_SOUND::Init's own pointer
    // parameter is a separate, unrelated call one level down and does not constrain this one).
    bool FindSoundByName(const sound_permutation *perm, int identifier,
                          const HALO_CHANNEL_PROPERTIES &channelProps,
                          HALO_SOUND_LIST **plist, int *fsbIndex,
                          const HALO_SOUND_PARAMS **psoundParams); // 0x836BE228
    void UnloadSound(const sound_permutation *sound);     // 0x836BE6B0 — stub (empty body)
    bool UpdateCustomSound(HALO_CUSTOM_SOUND &sound);       // 0x836BE6C4
    void UpdateCustomSounds();                               // 0x836BEA5C
    HALO_CHANNEL *GetChannel(int virtualChannelIndex, bool createNew); // 0x836BF00C
    void PrintUsedSounds(const char *prefix);                  // 0x836C13E8
    void Stop(int virtualChannelIndex);                         // 0x836C370C
    void PlayCustomSound(const char *soundDefName, const char *name); // 0x836C386C

    // DEVIATION: moved from `private` to `public` -- the HALO_SOUND_LIST/HALO_CHANNEL re-source
    // pass's own decompiles show both of these called from OTHER classes' methods
    // (HALO_SOUND_LIST::GetFreeSound calls `HALO_SOUND_SYSTEM::FindUnused(haloSoundSystem, this,
    // 1)`; HALO_CHANNEL::CheckNotReady calls `HALO_SOUND_SYSTEM::StealOldChannel(haloSoundSystem)`),
    // which cannot compile against a private member without a friend declaration the binary
    // shows no evidence of. Access specifiers aren't part of the mangled name, so this corrects
    // an editorial guess, not a DB fact.
    void StealOldChannel();                                     // 0x836BCAA0
    int  FindUnused(HALO_SOUND_LIST *soundList, bool unload);    // 0x836BEE90

private:
    void LoadTagParams();                                        // 0x836C3468
};

// 0x836B5580 — the nested listenerProps ctor's mangled top-level symbol
// (`??0<unnamed-type-listenerProps>@HALO_SOUND_SYSTEM@@QAA@XZ`) targets this inline ctor.

// Global singleton pointer set by the constructor (asserted single-instance outside of the
// `hkpCollisionAgent::removePoint`-gated editor/tool rebuild path). boundary — declared here,
// defined by whichever translation unit first constructs a HALO_SOUND_SYSTEM (matches the
// existing corpus convention for engine-wide singletons).
extern HALO_SOUND_SYSTEM *haloSoundSystem;
