#pragma once
// HCEX sound bridge: one active/preloaded playback slot for a Blam sound_permutation — the
// "current" and "queued" sub-objects embedded in HALO_CHANNEL, and the element type of
// HALO_SOUND_SYSTEM::preloadList.
// DB-verified layout (types_members HALO_PERM_SOUND): info@0 (sound_permutation const*),
// soundList@4 (HALO_SOUND_LIST*), fsbIndex@8, soundBank@12 (FMOD::Sound*),
// subSound@16 (FMOD::Sound*), channel@20 (FMOD::Channel*), previousChannel@24 (FMOD::Channel*),
// previousFreq@28, initTime@32, isUsed@36, wasUsed@37, prevFadeOut@40 (HALO_CROSSFADE_DSP*),
// fadeIn@44 (HALO_CROSSFADE_DSP*), params@48 (HALO_SOUND_PARAMS const*), tagInfo@52 (TAG_INFO*),
// identifier@56 — size 60.
// Sibling type: owned by the parallel HALO_CHANNEL drain; declared here from the DB only far
// enough to type HALO_SOUND_SYSTEM's field reads.

#include "../ws/snd/snd_fmod_boundary.h"

struct sound_permutation;   // Blam tag runtime object — full layout in ../sound_permutation.h
struct HALO_SOUND_LIST;     // full layout in HALO_SOUND_LIST.h
struct HALO_SOUND_PARAMS;   // full layout in HALO_SOUND_PARAMS.h
struct TAG_INFO;            // full layout in TAG_INFO.h
struct HALO_CROSSFADE_DSP;  // boundary — DSP crossfade helper, external to this batch

typedef struct HALO_PERM_SOUND {
    const sound_permutation *info;             // 0x00
    HALO_SOUND_LIST          *soundList;         // 0x04
    int                        fsbIndex;          // 0x08
    FMOD::Sound              *soundBank;          // 0x0C
    FMOD::Sound              *subSound;           // 0x10
    FMOD::Channel             *channel;            // 0x14
    FMOD::Channel             *previousChannel;    // 0x18
    float                      previousFreq;        // 0x1C
    int                        initTime;            // 0x20
    bool                       isUsed;              // 0x24
    bool                       wasUsed;             // 0x25
    unsigned char _pad0[2]; /* db-verified padding */
    HALO_CROSSFADE_DSP        *prevFadeOut;         // 0x28
    HALO_CROSSFADE_DSP        *fadeIn;              // 0x2C
    const HALO_SOUND_PARAMS   *params;              // 0x30
    TAG_INFO                  *tagInfo;              // 0x34
    int                        identifier;           // 0x38

    HALO_PERM_SOUND();  // ??0HALO_PERM_SOUND@@QAA@XZ
    ~HALO_PERM_SOUND(); // ??1HALO_PERM_SOUND@@QAA@XZ
    HALO_PERM_SOUND *scalarDeletingDtor(unsigned char deleteFlags); // ??_GHALO_PERM_SOUND@@QAAPAXI@Z

    bool  IsValid() const;                                            // ?IsValid@HALO_PERM_SOUND@@QBA_NXZ
    float GetVolume() const;                                          // ?GetVolume@HALO_PERM_SOUND@@QBAMXZ
    float GetVolume2D() const;                                        // ?GetVolume2D@HALO_PERM_SOUND@@QBAMXZ
    bool  IsPlayToEnd() const;                                        // ?IsPlayToEnd@HALO_PERM_SOUND@@QBA_NXZ
    void  Reset();                                                    // ?Reset@HALO_PERM_SOUND@@QAAXXZ
    // DEVIATION: the funcs.prototype convenience string types channelProps as
    // `const HALO_CHANNEL_PROPERTIES *`, but the mangled symbol's 3rd argument code (`ABU`)
    // decodes as reference-to-const; per project convention the mangled name is ground truth,
    // so this is `const HALO_CHANNEL_PROPERTIES &` (matches HALO_SOUND_SYSTEM::FindSoundByName,
    // to which this parameter is forwarded unchanged).
    void  Init(const sound_permutation *info, int identifier,
               const struct HALO_CHANNEL_PROPERTIES &channelProps);   // ?Init@HALO_PERM_SOUND@@QAAXPBUsound_permutation@@JABUHALO_CHANNEL_PROPERTIES@@@Z
    void  ResetFadeIn();                                              // ?ResetFadeIn@HALO_PERM_SOUND@@QAAXXZ
    void  Stop();                                                     // ?Stop@HALO_PERM_SOUND@@QAAXXZ
    void  UpdateStitchingDelay(bool isRecalc);                        // ?UpdateStitchingDelay@HALO_PERM_SOUND@@QAAX_N@Z
} HALO_PERM_SOUND;
