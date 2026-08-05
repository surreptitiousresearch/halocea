#include "../../headers/hcex/HALO_SOUND_SYSTEM.h"
#include "../../headers/hcex/HALO_SOUND_SYSTEM_globals.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

// _UnpauseChannel(FMOD::Channel*, int) @ 0x836B55C0
// Free helper (not a HALO_SOUND_SYSTEM method) that reapplies a channel's DSP-clock pause
// delay after the system resumes from Pause(true). Declared here only to type the two call
// sites in Pause(false); body is a sibling/free-function boundary (src/hcex/sound/_UnpauseChannel.cpp).
// NOTE: the standalone decompile renders a spurious 3rd (u64) parameter; disasm 0x836B55C0 shows
// only r3 (channel) + r4 (delayDspSamples) are incoming, so it is declared 2-arg.
extern "C" void UnpauseChannel(FMOD::Channel *channel, int delayDspSamples);

// HALO_SOUND_SYSTEM::Pause(bool) @ 0x836BCC44
// Freezes/thaws every live channel at the DSP-clock level: on pause, stamps `pauseTime` with
// "now + minDelay" and schedules every channel's current/queued FMOD::Channel to pause at
// that DSP clock tick; on resume, reads the current DSP clock and re-delays every channel by
// the elapsed pause duration (CAVEAT below).
void HALO_SOUND_SYSTEM::Pause(bool pause)
{
    if ((snd::System->state.val & 1) != 0 && snd::THREAD_ID != osGetCurThreadId()) {
        unsigned int curThreadId = osGetCurThreadId();
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
                             "HALO_SOUND_SYSTEM::Pause", snd::THREAD_ID, curThreadId);
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != osGetCurThreadId())
            STRONG_ASSERT_DUMMY().Crash("snd::THREAD_ID == osGetCurThreadId()",
                                         "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                                         833, empty_string);
        return;
    }

    if (isPaused == pause)
        return;

    FMOD::System *fmodSystem = system;
    isPaused = pause;

    if (pause) {
        FMOD_RESULT result = fmodSystem->getDSPClock(&pauseTime.hi, &pauseTime.lo);
        if (result) {
            if (result == FMOD_ERR_FILE_NOTFOUND && !IGNORE_STRONG_ASSERT)
                STRONG_ASSERT_DUMMY().Crash(
                    "!\"Sound file not found. Check perforce settings.\\nInfo: \" \"system->getDSPClock(&pauseTime.hi, &pauseTime.lo)\"",
                    "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 842, empty_string);
            const char *desc = FModErrorDesc(result, 0);
            osOutputDebugString("%s(%d): FMOD error 0x%08x (%s)\n",
                                 "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 842, result, desc);
        }

        // Schedule the pause to take effect `minDelay` DSP samples from now (hi:lo += minDelay,
        // with carry into hi on unsigned overflow of lo).
        unsigned int lo = pauseTime.lo;
        unsigned int hi = pauseTime.hi;
        unsigned int newLo = lo + minDelay;
        pauseTime.lo = newLo;
        pauseTime.hi = hi + (newLo < lo ? 1 : 0);

        for (int i = 0; i < channels.nElem; ++i) {
            HALO_CHANNEL &chan = channels[i];
            if (chan.current.channel)
                chan.current.channel->setDelay(FMOD_DELAYTYPE_DSPCLOCK_PAUSE, pauseTime.hi, pauseTime.lo);
            if (chan.queued.channel)
                chan.queued.channel->setDelay(FMOD_DELAYTYPE_DSPCLOCK_PAUSE, pauseTime.hi, pauseTime.lo);
        }
    } else {
        unsigned int curHi = 0, curLo = 0;
        FMOD_RESULT result = fmodSystem->getDSPClock(&curHi, &curLo);
        if (result) {
            if (result == FMOD_ERR_FILE_NOTFOUND && !IGNORE_STRONG_ASSERT)
                STRONG_ASSERT_DUMMY().Crash(
                    "!\"Sound file not found. Check perforce settings.\\nInfo: \" \"system->getDSPClock(&currentTime.hi, &currentTime.lo)\"",
                    "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 863, empty_string);
            const char *desc = FModErrorDesc(result, 0);
            osOutputDebugString("%s(%d): FMOD error 0x%08x (%s)\n",
                                 "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 863, result, desc);
        }

        // `delayMs` is how far past the scheduled pause point the DSP clock now sits (minDelay +
        // curLo, minus the pause target hi:lo, as a plain millisecond-scale delta), forwarded to
        // UnpauseChannel as its lone delay argument. (The decompile's third u64 argument is a
        // phantom — disasm 0x836B55C0 shows UnpauseChannel reads only r3+r4 — so it is dropped.)
        unsigned int targetLo = minDelay + curLo;
        int delayMs = (int)(targetLo - pauseTime.lo) - (int)pauseTime.hi;

        for (int i = 0; i < channels.nElem; ++i) {
            HALO_CHANNEL &chan = channels[i];
            UnpauseChannel(chan.current.channel, delayMs);
            UnpauseChannel(chan.queued.channel, delayMs);
        }
    }
}
