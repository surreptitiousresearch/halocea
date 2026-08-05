#pragma once
// HCEX sound bridge: one loaded FSB sound-list ("<level>.fsb"-style bank) — element type of
// HALO_SOUND_SYSTEM::soundLists (fixed 15-slot table, one per SOUND_LIST_LST entry).
// DB-verified layout (types_members HALO_SOUND_LIST): isMemory@0, isLoaded@1,
// levelName@4 (dsTSTRING<char>), fileName@8 (dsTSTRING<char>), tags@12
// (dsVECTOR<HALO_SOUND_TAG,8>), soundsCount@32, data@36 (unsigned char*), dataLength@40,
// streams@44 (dsVECTOR<FMOD::Sound*,8>), streamUsed@64 (dsVECTOR<bool,8>) — size 84.
// Source: D:\Projects\code\HCEX\sources\sound\fmod\sound_dsound_fmod.cpp.

#include <stdint.h>
#include "../ws/ds/dsTSTRING.h"
#include "../ws/ds/dsVECTOR.h"
#include "../ws/snd/snd_fmod_boundary.h"
#include "HALO_SOUND_TAG.h"

struct sound_permutation; // Blam tag runtime object — full layout in ../sound_permutation.h
struct HALO_CUSTOM_SOUND; // full layout in HALO_CUSTOM_SOUND.h

typedef struct HALO_SOUND_LIST {
    bool                          isMemory;    // 0x00 -- true: whole .fsb loaded into `data`/streams[0]; false: on-demand streams
    bool                          isLoaded;     // 0x01 -- true once Preload()'d (isMemory) or ready (streaming)
    uint8_t               _pad02[2];    // 0x02
    dsTSTRING<char>                levelName;    // 0x04
    dsTSTRING<char>                fileName;     // 0x08 -- the .fsb path
    dsVECTOR<HALO_SOUND_TAG, 8>    tags;          // 0x0C -- sorted by name; binary-searched by Find
    int                            soundsCount;  // 0x20 -- total sub-sound count across all tags
    uint8_t               *data;         // 0x24 -- isMemory: the whole .fsb file, owned (new[]/delete[])
    int                            dataLength;   // 0x28 -- isMemory: byte length of `data`
    dsVECTOR<FMOD::Sound *, 8>     streams;       // 0x2C -- isMemory: streams[0] is the one bank; else: one stream per prepared voice
    dsVECTOR<bool, 8>              streamUsed;    // 0x40 -- streaming: per-stream in-use flag

    HALO_SOUND_LIST();  // ??0HALO_SOUND_LIST@@QAA@XZ
    ~HALO_SOUND_LIST(); // ??1HALO_SOUND_LIST@@QAA@XZ
    HALO_SOUND_LIST *scalarDeletingDtor(unsigned char deleteFlags); // ??_GHALO_SOUND_LIST@@QAAPAXI@Z

    // Load `lst` (a .lst-bin if present, else a text .lst) describing `fsb`'s tag table, then
    // resolve each tag's HALO_SOUND_PARAMS and, if prepareStreams>0, open that many streaming
    // voices up front. levelName is stashed for diagnostics only.
    void Init(const char *levelName, const char *lst, const char *fsb, int prepareStreams); // ?Init@HALO_SOUND_LIST@@QAAXPBD00H@Z

    // Binary search `tags` (sorted by name) for `tag_name`; NULL if not found, isMemory && !isLoaded
    // yet, or off the audio thread with the strong-assert path taken.
    const HALO_SOUND_TAG *Find(const char *tag_name) const; // ?Find@HALO_SOUND_LIST@@QBAPBUHALO_SOUND_TAG@@PBD@Z

    // Fully unload an isMemory bank (release the one FMOD::Sound, free `data`); a no-op unless
    // isLoaded && isMemory.
    void Unload(); // ?Unload@HALO_SOUND_LIST@@QAAXXZ

    // Release every stream/bank and reset all live counts to zero (used at map teardown).
    void Term(); // ?Term@HALO_SOUND_LIST@@QAAXXZ

    // isMemory: synchronously load the whole .fsb into `data`, create its one FMOD::Sound and
    // wait for it to finish opening. A no-op once isLoaded.
    void Preload(); // ?Preload@HALO_SOUND_LIST@@QAAXXZ

    // Pick a free (or freshly-created) FMOD::Sound to hand a channel: isMemory always returns
    // streams[0]; otherwise scans streamUsed for an idle, already-ready stream, else steals a
    // still-loading one, else (if tryToUsePreloaded fails to reclaim one via
    // HALO_SOUND_SYSTEM::FindUnused) opens a brand new stream and appends it.
    FMOD::Sound *GetFreeSound(bool tryToUsePreloaded); // ?GetFreeSound@HALO_SOUND_LIST@@QAAPAVSound@FMOD@@_N@Z

    // Mark the stream backing `sound` as free again (streaming case); isMemory only strong-
    // asserts streams[0] == sound (nothing to release -- the one bank stays resident).
    void ReleaseSound(FMOD::Sound *sound); // ?ReleaseSound@HALO_SOUND_LIST@@QAAXPAVSound@FMOD@@@Z

private:
    // Open (or create) one FMOD::Sound: isMemory calls FMOD::System::createSound over `data`/
    // `dataLength` (FMOD_OPENMEMORY_POINT | FMOD_CREATECOMPRESSEDSAMPLE); otherwise
    // FMOD::System::createStream over `fileName`. Both use FMOD_NONBLOCKING unless
    // snd::SystemFMod->dbg.disableNonblocking (or its debug bit) forces a blocking open.
    FMOD::Sound *CreateSound(); // ?CreateSound@HALO_SOUND_LIST@@AAAPAVSound@FMOD@@XZ

    // Try the binary .lst-bin format first (fast path, versioned; returns false on an old/
    // missing file so the caller falls back to ParseLst). Populates `tags` directly from the
    // serialized (name,startIndex,count) triples.
    bool ParseBin(const char *lst); // ?ParseBin@HALO_SOUND_LIST@@AAA_NPBD@Z

    // Slow-path text .lst parser: one "sound\<name>.<ext>" relative path per line: derives each
    // sub-sound's lowercased tag name from the path, groups identical tag names into a
    // contiguous FSB sub-sound range, and rebuilds `tags` sorted by name.
    void ParseLst(const char *lst); // ?ParseLst@HALO_SOUND_LIST@@AAAXPBD@Z

    // After `tags` is populated (by ParseBin/ParseLst), resolve each tag's HALO_SOUND_PARAMS:
    // exact-name match against haloSoundSystem->params first, else the first matching
    // ds::WILDCARD pattern in haloSoundSystem->paramsWildcard.
    void InitSetupTagParams(); // ?InitSetupTagParams@HALO_SOUND_LIST@@AAAXXZ
} HALO_SOUND_LIST;
