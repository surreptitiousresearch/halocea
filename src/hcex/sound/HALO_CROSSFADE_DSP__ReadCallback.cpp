// ?ReadCallback@HALO_CROSSFADE_DSP@@SA?AW4FMOD_RESULT@@PAUFMOD_DSP_STATE@@PAM1IHH@Z -- 0x836B8FD0
//
// CAVEAT: the decompiler flags this function "local variable allocation has failed" and packs
// the 64-bit DSP-clock/window-boundary values into ad-hoc HIDWORD/LODWORD register slots.
// Reconstructed from the disassembly: the overlap test, the ramp-direction selection, the
// log2-domain per-sample gain curve, and the per-channel multiply loop are all disasm-
// confirmed. The two decompiled "boundary gain" computations (one skips the ratio divide when
// the window starts exactly at this block's first affected sample) are unified below into one
// general lerp formula -- mathematically equivalent, since the skipped branch is just that
// formula's ratio=0 case.
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "../../headers/hcex/HALO_CROSSFADE_DSP.h"

extern "C" const char *FModErrorDesc(FMOD_RESULT res, bool info);
extern void _apLog(const char *fmt, ...);
extern "C" void         osOutputDebugString(const char *fmt, ...);
extern "C" double       __u64tod(uint64_t value); // boundary -- PPC runtime helper
extern const float      inv_log_2;                        // boundary -- 1/ln(2), named global

FMOD_RESULT __fastcall HALO_CROSSFADE_DSP::ReadCallback(FMOD_DSP_STATE *dsp_state, float *inbuffer,
                                                          float *outbuffer, unsigned int length,
                                                          int inchannels, int outchannels)
{
    FMOD::DSP *instance = dsp_state->instance;

    void *userdata = nullptr;
    FMOD_RESULT res = instance->getUserData(&userdata);
    if (res)
    {
        _apLog("~Error,Sound,FMOD~%s: \"%s\" (line %d)", FModErrorDesc(res, false),
               "dsp->getUserData(&userdata)", 474);
        osOutputDebugString("~Error,Sound,FMOD~%s: \"%s\" (line %d)\n", FModErrorDesc(res, false),
                            "dsp->getUserData(&userdata)", 474);
    }
    HALO_CROSSFADE_DSP *state = static_cast<HALO_CROSSFADE_DSP *>(userdata);

    FMOD::System *system = nullptr;
    res = instance->getSystemObject(&system);
    if (res)
    {
        _apLog("~Error,Sound,FMOD~%s: \"%s\" (line %d)", FModErrorDesc(res, false),
               "dsp->getSystemObject(&system)", 479);
        osOutputDebugString("~Error,Sound,FMOD~%s: \"%s\" (line %d)\n", FModErrorDesc(res, false),
                            "dsp->getSystemObject(&system)", 479);
    }

    unsigned int blockStartHi = 0, blockStartLo = 0;
    res = system->getDSPClock(&blockStartHi, &blockStartLo);
    if (res)
    {
        _apLog("~Error,Sound,FMOD~%s: \"%s\" (line %d)", FModErrorDesc(res, false),
               "system->getDSPClock(&blockStart.hi, &blockStart.lo)", 483);
        osOutputDebugString("~Error,Sound,FMOD~%s: \"%s\" (line %d)\n", FModErrorDesc(res, false),
                            "system->getDSPClock(&blockStart.hi, &blockStart.lo)", 483);
    }

    // Dry pass-through first; the gain ramp below (if the crossfade window overlaps this block)
    // is applied to outbuffer in place afterward.
    memcpy(outbuffer, inbuffer, sizeof(float) * length * inchannels);

    uint64_t blockStart = ((uint64_t)blockStartHi << 32) | blockStartLo;
    uint64_t blockEnd = blockStart + (length - 1);

    if (state->startTime > blockEnd || state->endTime < blockStart)
        return FMOD_OK; // crossfade window doesn't overlap this block at all

    // isFadeIn selects the ramp direction: true ramps 1.0 -> 0.0 (the outgoing/previous
    // channel's DSP), false ramps 0.0 -> 1.0 (the incoming/current channel's DSP).
    float levelAtRatio0 = state->isFadeIn ? 1.0f : 0.0f;
    float levelAtRatio1 = state->isFadeIn ? 0.0f : 1.0f;

    double windowDuration = __u64tod(state->endTime - state->startTime);

    uint64_t windowStartAbs = (state->startTime > blockStart) ? state->startTime : blockStart;
    uint64_t windowEndAbs = (state->endTime < blockEnd) ? state->endTime : blockEnd;

    unsigned int windowStartSample = (unsigned int)(windowStartAbs - blockStart);
    unsigned int windowEndSample = (unsigned int)(windowEndAbs - blockStart);

    float ratioAtStart = (float)(__u64tod(windowStartAbs - state->startTime) / windowDuration);
    float ratioAtEnd = (float)(__u64tod(windowEndAbs - state->startTime) / windowDuration);

    float gainAtWindowStart = (1.0f - ratioAtStart) * levelAtRatio0 + ratioAtStart * levelAtRatio1;
    float gainAtWindowEnd = (1.0f - ratioAtEnd) * levelAtRatio0 + ratioAtEnd * levelAtRatio1;

    if (windowStartSample > windowEndSample)
        return FMOD_OK;

    float gainDelta = gainAtWindowEnd - gainAtWindowStart;
    unsigned int windowSampleCount = windowEndSample - windowStartSample + 1;
    float windowSampleCountAsFloat = (float)windowSampleCount;
    int elemIndexBase = windowStartSample * inchannels;

    for (unsigned int sampleIndex = windowStartSample; sampleIndex <= windowEndSample; ++sampleIndex)
    {
        unsigned int sampleOffsetInWindow = sampleIndex - windowStartSample + 1;
        float x = (float)sampleOffsetInWindow * gainDelta / windowSampleCountAsFloat + gainAtWindowStart + 1.0f;
        float scale = (float)(log(x) * inv_log_2); // log2(x)

        // DEVIATION: a STRONG_ASSERT2_HELPER::asserd<int,float,float> debug-only finiteness
        // check on `scale * outbuffer[idx]` (via _m3dCheckValid) runs here per-sample in the
        // binary; it only logs and never changes control flow, so it's elided from this
        // reconstruction.
        for (int ch = 0; ch < inchannels; ++ch)
        {
            int idx = elemIndexBase + ch;
            outbuffer[idx] = scale * outbuffer[idx];
        }
        elemIndexBase += inchannels;
    }

    return FMOD_OK;
}
