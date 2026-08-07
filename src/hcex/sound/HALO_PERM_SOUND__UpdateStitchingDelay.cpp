// ?UpdateStitchingDelay@HALO_PERM_SOUND@@QAAX_N@Z -- 0x836C2530
//
// CAVEAT: the decompiler flags this function "local variable allocation has failed" and
// packs the FMOD 64-bit DSP-clock/delay values (which this ABI splits into hi:lo GPR pairs)
// into ad-hoc HIDWORD/LODWORD slots that obscure the real arithmetic. Reconstructed here from
// the disassembly store-by-store: every hi:lo pair is modeled as a real `unsigned __int64`
// (combining hi<<32|lo, matching how the disassembly itself loads a hi:lo pair with a single
// 64-bit `ld` once they're stored contiguously) rather than reproducing the raw carry-flag
// juggling instruction-for-instruction -- semantically identical, since a hi:lo GPR pair IS a
// 64-bit value on this ABI, just verified independently for each of the three code paths below.
#include <stdint.h>
#include "../../headers/hcex/HALO_PERM_SOUND.h"
#include "../../headers/hcex/HALO_SOUND_SYSTEM.h"
#include "../../headers/hcex/HALO_CROSSFADE_DSP.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"
#include "../../headers/ws/wb/dbgVAR_boundary.h"

extern const char *FModErrorDesc(FMOD_RESULT res, bool info); // C++ linkage: ?FModErrorDesc@@YAPBDW4FMOD_RESULT@@_N@Z
extern "C" void         osOutputDebugString(const char *fmt, ...);
extern void _apLog(const char *fmt, ...);

// Debug-tunable crossfade window length, in output-rate samples (dbgVAR_SIMPLE<int,2>).
extern "C" dbgVAR_SIMPLE<int, 2> CROSSFADE_TIME;

void HALO_PERM_SOUND::UpdateStitchingDelay(bool isRecalc)
{
    FMOD::Sound *previousSound = nullptr;
    float previousFrequency = 0.0f;
    if (previousChannel)
    {
        previousChannel->getCurrentSound(&previousSound);
        previousChannel->getFrequency(&previousFrequency);
    }

    unsigned int delayHi = 0, delayLo = 0;

    // 0.000001f -- the engine's M3D_EPSILON tolerance (duplicated as a per-reference constant
    // pool entry throughout the binary rather than one named global).
    if (previousSound && previousFrequency >= 0.000001f)
    {
        previousFreq = previousFrequency;
        if (!IGNORE_STRONG_ASSERT && !previousChannel)
            STRONG_ASSERT_DUMMY().Crash(
                "previousChannel != 0",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                1823,
                empty_string);

        unsigned int lengthPcm = 0;
        FMOD_RESULT res = previousSound->getLength(&lengthPcm, FMOD_TIMEUNIT_PCM);
        if (res)
        {
            if (res == FMOD_ERR_FILE_NOTFOUND && !IGNORE_STRONG_ASSERT)
                STRONG_ASSERT_DUMMY().Crash(
                    "!\"Sound file not found. Check perforce settings.\\nInfo: \" \"subSound->getLength(&lengthPcm, FMOD_TIMEUNIT_PCM)\"",
                    "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                    1826,
                    empty_string);
            osOutputDebugString("%s(%d): FMOD error 0x%08x (%s)\n",
                                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                                1826, res, FModErrorDesc(res, false));
        }

        if (isRecalc)
        {
            // Previously stitched: the transition is already in flight, only the delay target
            // needs recomputing to account for drift -- keep both crossfade DSPs bypassed while
            // we do the math.
            if (!IGNORE_STRONG_ASSERT && (!prevFadeOut || !fadeIn))
                STRONG_ASSERT_DUMMY().Crash(
                    "prevFadeOut != 0 && fadeIn != 0",
                    "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                    1844,
                    empty_string);
            prevFadeOut->dsp->setBypass(true);
            fadeIn->dsp->setBypass(true);

            unsigned int dspClockHi = 0, dspClockLo = 0;
            res = haloSoundSystem->system->getDSPClock(&dspClockHi, &dspClockLo);
            if (res)
            {
                if (res == FMOD_ERR_FILE_NOTFOUND && !IGNORE_STRONG_ASSERT)
                    STRONG_ASSERT_DUMMY().Crash(
                        "!\"Sound file not found. Check perforce settings.\\nInfo: \" \"haloSoundSystem->system->getDSPClock(&startTime.hi, &startTime.lo)\"",
                        "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                        1848,
                        empty_string);
                osOutputDebugString("%s(%d): FMOD error 0x%08x (%s)\n",
                                    "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                                    1848, res, FModErrorDesc(res, false));
            }

            unsigned int positionPcm = 0;
            res = previousChannel->getPosition(&positionPcm, FMOD_TIMEUNIT_PCM);
            if (res)
            {
                if (res == FMOD_ERR_FILE_NOTFOUND && !IGNORE_STRONG_ASSERT)
                    STRONG_ASSERT_DUMMY().Crash(
                        "!\"Sound file not found. Check perforce settings.\\nInfo: \" \"previousChannel->getPosition(&positionPcm, FMOD_TIMEUNIT_PCM)\"",
                        "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                        1851,
                        empty_string);
                osOutputDebugString("%s(%d): FMOD error 0x%08x (%s)\n",
                                    "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                                    1851, res, FModErrorDesc(res, false));
            }

            unsigned int remainingPcm = lengthPcm - positionPcm;
            int64_t remainingAtOutputRate =
                (int64_t)((float)((float)haloSoundSystem->outputRate / previousFrequency) * (float)remainingPcm + 0.5f);

            uint64_t dspClock = ((uint64_t)dspClockHi << 32) | dspClockLo;
            uint64_t delay = dspClock + (unsigned int)remainingAtOutputRate;
            delayHi = (unsigned int)(delay >> 32);
            delayLo = (unsigned int)delay;
        }
        else
        {
            // First-time stitch: acquire the crossfade DSP pair, wire them onto the outgoing
            // (previous) and incoming (current) channels, and target the delay so the new
            // channel starts CROSSFADE_TIME samples before the previous one's remaining length
            // runs out.
            if (!IGNORE_STRONG_ASSERT && (prevFadeOut || fadeIn))
                STRONG_ASSERT_DUMMY().Crash(
                    "prevFadeOut == 0 && fadeIn == 0",
                    "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                    1829,
                    empty_string);

            prevFadeOut = HALO_CROSSFADE_DSP::Acquire();
            fadeIn = HALO_CROSSFADE_DSP::Acquire();

            res = previousChannel->addDSP(prevFadeOut->dsp, nullptr);
            if (res)
            {
                _apLog("~Error,Sound,FMOD~%s: \"%s\" (line %d)", FModErrorDesc(res, false),
                       "previousChannel->addDSP(prevFadeOut->dsp, NULL)", 1833);
                osOutputDebugString("~Error,Sound,FMOD~%s: \"%s\" (line %d)\n", FModErrorDesc(res, false),
                                    "previousChannel->addDSP(prevFadeOut->dsp, NULL)", 1833);
            }
            res = channel->addDSP(fadeIn->dsp, nullptr);
            if (res)
            {
                _apLog("~Error,Sound,FMOD~%s: \"%s\" (line %d)", FModErrorDesc(res, false),
                       "channel->addDSP(fadeIn->dsp, NULL)", 1834);
                osOutputDebugString("~Error,Sound,FMOD~%s: \"%s\" (line %d)\n", FModErrorDesc(res, false),
                                    "channel->addDSP(fadeIn->dsp, NULL)", 1834);
            }

            unsigned int existingDelayHi = 0, existingDelayLo = 0;
            res = previousChannel->getDelay(FMOD_DELAYTYPE_DSPCLOCK_START, &existingDelayHi, &existingDelayLo);
            if (res)
            {
                if (res == FMOD_ERR_FILE_NOTFOUND && !IGNORE_STRONG_ASSERT)
                    STRONG_ASSERT_DUMMY().Crash(
                        "!\"Sound file not found. Check perforce settings.\\nInfo: \" \"previousChannel->getDelay(FMOD_DELAYTYPE_DSPCLOCK_START, &startTime.hi, &startTime.lo)\"",
                        "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                        1836,
                        empty_string);
                osOutputDebugString("%s(%d): FMOD error 0x%08x (%s)\n",
                                    "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                                    1836, res, FModErrorDesc(res, false));
            }

            int64_t lengthAtOutputRate =
                (int64_t)((float)((float)haloSoundSystem->outputRate / previousFrequency) * (float)lengthPcm + 0.5f);

            if (!IGNORE_STRONG_ASSERT && (unsigned int)lengthAtOutputRate <= (unsigned int)CROSSFADE_TIME.value)
                STRONG_ASSERT_DUMMY().Crash(
                    "lengthPcm > CROSSFADE_TIME",
                    "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                    1841,
                    empty_string);

            uint64_t existingDelay = ((uint64_t)existingDelayHi << 32) | existingDelayLo;
            uint64_t stitchDelay = existingDelay + ((unsigned int)lengthAtOutputRate - (unsigned int)CROSSFADE_TIME.value);
            delayHi = (unsigned int)(stitchDelay >> 32);
            delayLo = (unsigned int)stitchDelay;
        }
    }
    else
    {
        // No previous channel to stitch from -- just target a plain startup delay of
        // "now + minDelay", unless this is only a recalc (nothing to recalc without a previous
        // channel).
        if (isRecalc)
            return;

        if (prevFadeOut)
        {
            prevFadeOut->Detach();
            prevFadeOut->Release();
            prevFadeOut = nullptr;
        }
        if (fadeIn)
        {
            fadeIn->Detach();
            fadeIn->Release();
            fadeIn = nullptr;
        }

        unsigned int dspClockHi = 0, dspClockLo = 0;
        FMOD_RESULT res = haloSoundSystem->system->getDSPClock(&dspClockHi, &dspClockLo);
        if (res)
        {
            if (res == FMOD_ERR_FILE_NOTFOUND && !IGNORE_STRONG_ASSERT)
                STRONG_ASSERT_DUMMY().Crash(
                    "!\"Sound file not found. Check perforce settings.\\nInfo: \" \"haloSoundSystem->system->getDSPClock(&startTime.hi, &startTime.lo)\"",
                    "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                    1818,
                    empty_string);
            osOutputDebugString("%s(%d): FMOD error 0x%08x (%s)\n",
                                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                                1818, res, FModErrorDesc(res, false));
        }

        uint64_t dspClock = ((uint64_t)dspClockHi << 32) | dspClockLo;
        uint64_t delay = dspClock + haloSoundSystem->minDelay;
        delayHi = (unsigned int)(delay >> 32);
        delayLo = (unsigned int)delay;
    }

    channel->setDelay(FMOD_DELAYTYPE_DSPCLOCK_START, delayHi, delayLo);

    // Only on the first-time stitch (never on a recalc) do we (re)set the crossfade windows and
    // let the two DSPs start processing.
    if (fadeIn && !isRecalc)
    {
        uint64_t delay64 = ((uint64_t)delayHi << 32) | delayLo;

        prevFadeOut->startTime = delay64;
        prevFadeOut->isFadeIn = false;
        prevFadeOut->endTime = delay64 + (unsigned int)CROSSFADE_TIME.value;

        fadeIn->isFadeIn = true;
        fadeIn->startTime = delay64;
        fadeIn->endTime = delay64 + (unsigned int)CROSSFADE_TIME.value;

        prevFadeOut->dsp->setBypass(false);
        fadeIn->dsp->setBypass(false);
    }
}
