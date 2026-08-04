#pragma once
// HCEX sound bridge: a one-off "custom sound" playback slot (UI/scripted sounds played by
// filename rather than by Blam tag) — element type of HALO_SOUND_SYSTEM::customSounds.
// DB-verified layout (types_members HALO_CUSTOM_SOUND): soundList@0 (HALO_SOUND_LIST*),
// soundBank@4 (FMOD::Sound*), fsbIndex@8, subSound@12 (FMOD::Sound*), channel@16
// (FMOD::Channel*) — size 20.
// Sibling type: owned by the parallel HALO_SOUND_LIST drain; declared here from the DB only
// far enough to type HALO_SOUND_SYSTEM's field reads.

#include "../ws/snd/snd_fmod_boundary.h"

struct HALO_SOUND_LIST; // full layout in HALO_SOUND_LIST.h

typedef struct HALO_CUSTOM_SOUND {
    HALO_SOUND_LIST *soundList; // 0x00
    FMOD::Sound      *soundBank; // 0x04
    int                fsbIndex;  // 0x08
    FMOD::Sound      *subSound;   // 0x0C
    FMOD::Channel     *channel;    // 0x10

    HALO_CUSTOM_SOUND(); // ??0HALO_CUSTOM_SOUND@@QAA@XZ
} HALO_CUSTOM_SOUND;
