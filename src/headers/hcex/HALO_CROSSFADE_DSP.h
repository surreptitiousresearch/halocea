#pragma once
// HCEX sound bridge: a refcounted, pooled FMOD custom DSP unit ("HCEX_Crossfade") that
// applies a log-domain linear-gain ramp over the DSP-clock window [startTime,endTime),
// used by HALO_PERM_SOUND to stitch a looping permutation's playback across channels with
// no audible seam. Instances are recycled through a static free-list (HALO_CROSSFADE_DSP::
// cache) via Acquire()/Release() rather than newed/deleted per use.
// DB-verified layout (types_members HALO_CROSSFADE_DSP): startTime@0, endTime@8, refCount@16,
// dsp@20 (FMOD::DSP*), isFadeIn@24 — size 25 (padded to 28).

#include <stdint.h>
#include "../ws/snd/snd_fmod_boundary.h"
#include "../ws/ds/dsVECTOR.h"

typedef struct HALO_CROSSFADE_DSP {
    uint64_t startTime; // 0x00 -- FMOD DSP-clock tick the ramp begins at
    uint64_t endTime;   // 0x08 -- FMOD DSP-clock tick the ramp ends at
    int                refCount;  // 0x10
    FMOD::DSP         *dsp;        // 0x14 -- the underlying FMOD custom DSP unit
    bool               isFadeIn;   // 0x18 -- selects the ramp direction in ReadCallback (see .cpp)

    // Static free-list of released instances, reused by Acquire() before falling back to a
    // fresh allocation.
    static dsVECTOR<HALO_CROSSFADE_DSP *, 8> cache;

    HALO_CROSSFADE_DSP(); // ??0HALO_CROSSFADE_DSP@@QAA@XZ

    static HALO_CROSSFADE_DSP *Acquire();                   // ?Acquire@HALO_CROSSFADE_DSP@@SAPAU1@XZ
    // DEVIATION: decompiler shows `HALO_CROSSFADE_DSP **ptr` (its convenience prototype string
    // always renders references as pointers); the mangle's `AAPAU1@@` decodes as a reference to
    // a pointer, so the true parameter type is `HALO_CROSSFADE_DSP *&ptr`.
    static void DetachAndRelease(HALO_CROSSFADE_DSP *&ptr); // ?DetachAndRelease@HALO_CROSSFADE_DSP@@SAXAAPAU1@@Z

    void AddRef();  // ?AddRef@HALO_CROSSFADE_DSP@@QAAXXZ
    void Detach();  // ?Detach@HALO_CROSSFADE_DSP@@QAAXXZ
    void Release(); // ?Release@HALO_CROSSFADE_DSP@@QAAXXZ

    // Sets the ramp window: startTime = start, endTime = start + fadeTime, isFadeIn = isFadeIn.
    // DEVIATION: the decompiler mis-parsed this as a 4-arg call with a phantom leading int;
    // disasm confirms the DB's 3-arg (start,fadeTime,isFadeIn) prototype -- `start` (a 64-bit
    // DSP-clock tick) is passed whole in a single 64-bit GPR (this ABI's convention for a
    // 64-bit arg, matching the hi:lo pairs FMOD::System::getDSPClock fills elsewhere).
    void SetSettings(uint64_t start, int fadeTime, bool isFadeIn); // ?SetSettings@HALO_CROSSFADE_DSP@@QAAX_KH_N@Z

    // FMOD_DSP_DESCRIPTION::read callback -- applies the gain ramp to `outbuffer` in place
    // for the block [blockStart, blockStart+length) if it overlaps [startTime,endTime).
    static FMOD_RESULT __fastcall ReadCallback(FMOD_DSP_STATE *dsp_state, float *inbuffer,
                                                float *outbuffer, unsigned int length,
                                                int inchannels, int outchannels); // ?ReadCallback@HALO_CROSSFADE_DSP@@SA?AW4FMOD_RESULT@@PAUFMOD_DSP_STATE@@PAM1IHH@Z

    // FMOD_DSP_DESCRIPTION::release callback -- frees the userdata block FMOD handed back.
    static FMOD_RESULT __fastcall ReleaseCallback(FMOD_DSP_STATE *dsp_state); // ?ReleaseCallback@HALO_CROSSFADE_DSP@@SA?AW4FMOD_RESULT@@PAUFMOD_DSP_STATE@@@Z
} HALO_CROSSFADE_DSP;
