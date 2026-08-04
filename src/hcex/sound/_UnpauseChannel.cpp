/* _UnpauseChannel @0x836B55C0 — free helper (not a HALO_SOUND_SYSTEM method) that resumes a single
 * FMOD channel after HALO_SOUND_SYSTEM::Pause(true). Reads the channel's currently scheduled
 * DSPCLOCK_START delay, pushes it forward by `delayDspSamples` (the elapsed pause duration in DSP
 * samples, computed by the caller), clears the DSPCLOCK_PAUSE point, and unpauses the channel.
 * Every FMOD call is wrapped in the shipped strong-assert + osOutputDebugString error path.
 * Its two call sites live in HALO_SOUND_SYSTEM::Pause(false) (HALO_SOUND_SYSTEM__Pause.cpp).
 *
 * DEVIATION: Hex-Rays rendered the 64-bit unpauseTime += delayDspSamples carry as a pointer-index
 * pun on `channel` ("v30 += &channel[v10 & 1]") because the carry register aliased the channel
 * register across the block. Reconstructed here as a plain unsigned 64-bit add of the signed
 * sample delta (matching the DSP-clock hi:lo semantics). The standalone decompile also manufactured
 * a spurious 3rd (u64) parameter; disasm 0x836B55C0 shows only r3 (channel) and r4 (delayDspSamples)
 * are incoming, so the phantom parameter is dropped and the signature is 2-arg. */

#include "../../headers/hcex/HALO_SOUND_SYSTEM.h"
#include "../../headers/hcex/HALO_SOUND_SYSTEM_globals.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

static void report_fmod_error(FMOD_RESULT result, int line, const char *info)
{
    if (result == FMOD_ERR_FILE_NOTFOUND && !IGNORE_STRONG_ASSERT)
        STRONG_ASSERT_DUMMY().Crash(info,
            "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", line, dsStrongAssertMessage);
    const char *desc = FModErrorDesc(result, 0);
    osOutputDebugString("%s(%d): FMOD error 0x%08x (%s)\n",
        "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", line, result, desc);
}

extern "C" void UnpauseChannel(FMOD::Channel *channel, int delayDspSamples)
{
    if (!channel)
        return;

    unsigned int unpauseHi = 0;
    unsigned int unpauseLo = 0;

    FMOD_RESULT result = channel->getDelay(FMOD_DELAYTYPE_DSPCLOCK_START, &unpauseHi, &unpauseLo);
    if (result)
        report_fmod_error(result, 815,
            "!\"Sound file not found. Check perforce settings.\\nInfo: \" "
            "\"channel->getDelay(FMOD_DELAYTYPE_DSPCLOCK_START, &unpauseTime.hi, &unpauseTime.lo)\"");

    // 64-bit DSP-clock advance: unpauseTime += delayDspSamples (sign-extended), with carry into hi.
    unsigned int newLo = unpauseLo + (unsigned int)delayDspSamples;
    unsigned int carry = (newLo < unpauseLo) ? 1u : 0u;
    unpauseHi += carry + (delayDspSamples < 0 ? 0xFFFFFFFFu : 0u);
    unpauseLo = newLo;

    result = channel->setDelay(FMOD_DELAYTYPE_DSPCLOCK_START, unpauseHi, unpauseLo);
    if (result)
        report_fmod_error(result, 817,
            "!\"Sound file not found. Check perforce settings.\\nInfo: \" "
            "\"channel->setDelay(FMOD_DELAYTYPE_DSPCLOCK_START, unpauseTime.hi, unpauseTime.lo)\"");

    result = channel->setDelay(FMOD_DELAYTYPE_DSPCLOCK_PAUSE, 0, 0);
    if (result)
        report_fmod_error(result, 819,
            "!\"Sound file not found. Check perforce settings.\\nInfo: \" "
            "\"channel->setDelay(FMOD_DELAYTYPE_DSPCLOCK_PAUSE, 0, 0)\"");

    result = channel->setPaused(false);
    if (result)
        report_fmod_error(result, 820,
            "!\"Sound file not found. Check perforce settings.\\nInfo: \" "
            "\"channel->setPaused(false)\"");
}
