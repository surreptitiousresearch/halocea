#include "../headers/hcex/HALO_SOUND_SYSTEM.h"
#include "../headers/hcex/HALO_SOUND_SYSTEM_globals.h"
#include "../headers/ws/snd/snd_fmod_boundary.h"
#include "../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"
#include "../headers/ws/ds/ds_assert_boundary.h"

extern const char empty_string; // shared single-NUL assert-info constant @0x8200155A

// 0x836B55C0 — file-local FMOD helper: cancel a channel's scheduled pause and resume playback.
// DEVIATION: the standalone decompile of _UnpauseChannel renders a spurious 3rd (u64) parameter,
// but both call sites in Pause() pass exactly two registers (channel, delta), so it is declared
// here to match the actual call ABI.
extern void UnpauseChannel(FMOD::Channel *channel, int delta);

// 0x836BCC08 — pause/resume the whole Halo mixer. Must run on the owning sound thread (else log +
// assert). On pause: snapshot the DSP clock into pauseTime, offset it by minDelay, and schedule a
// FMOD_DELAYTYPE_DSPCLOCK_PAUSE on every live channel's current+queued FMOD channel. On resume:
// read the DSP clock, offset by minDelay, and UnpauseChannel every current+queued channel.
void HALO_SOUND_SYSTEM::Pause(bool pause)
{
    if ((snd::System->state.val & 1) != 0 && snd::THREAD_ID != osGetCurThreadId())
    {
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
            "HALO_SOUND_SYSTEM::Pause", snd::THREAD_ID, osGetCurThreadId());
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != osGetCurThreadId())
            STRONG_ASSERT_DUMMY::Crash(nullptr,
                "snd::THREAD_ID == osGetCurThreadId()",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                833, &empty_string);
        return;
    }

    if (this->isPaused == pause)
        return;

    FMOD::System *system = this->system;
    this->isPaused = pause;

    if (pause)
    {
        FMOD_RESULT res = system->getDSPClock(&this->pauseTime.hi, &this->pauseTime.lo);
        if (res)
        {
            if (res == FMOD_ERR_FILE_NOTFOUND && !IGNORE_STRONG_ASSERT)
                STRONG_ASSERT_DUMMY::Crash(nullptr,
                    "!\"Sound file not found. Check perforce settings.\\nInfo: \" \"system->getDSPClock(&pauseTime.hi, &pauseTime.lo)\"",
                    "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                    842, &empty_string);
            osOutputDebugString("%s(%d): FMOD error 0x%08x (%s)\n",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 842, res, FModErrorDesc(res, false));
        }

        // pauseTime += minDelay (64-bit, with carry into hi)
        unsigned int lo = this->pauseTime.lo;
        unsigned int newLo = lo + this->minDelay;
        this->pauseTime.lo = newLo;
        this->pauseTime.hi += (newLo < lo);

        for (int i = 0; i < this->channels.nElem; ++i)
        {
            if (this->channels[i].current.channel)
                this->channels[i].current.channel->setDelay(
                    FMOD_DELAYTYPE_DSPCLOCK_PAUSE, this->pauseTime.hi, this->pauseTime.lo);
            if (this->channels[i].queued.channel)
                this->channels[i].queued.channel->setDelay(
                    FMOD_DELAYTYPE_DSPCLOCK_PAUSE, this->pauseTime.hi, this->pauseTime.lo);
        }
    }
    else
    {
        unsigned int hi = 0, lo = 0;
        FMOD_RESULT res = system->getDSPClock(&hi, &lo);
        if (res)
        {
            if (res == FMOD_ERR_FILE_NOTFOUND && !IGNORE_STRONG_ASSERT)
                STRONG_ASSERT_DUMMY::Crash(nullptr,
                    "!\"Sound file not found. Check perforce settings.\\nInfo: \" \"system->getDSPClock(&currentTime.hi, &currentTime.lo)\"",
                    "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                    863, &empty_string);
            osOutputDebugString("%s(%d): FMOD error 0x%08x (%s)\n",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 863, res, FModErrorDesc(res, false));
        }

        // currentTime += minDelay (64-bit); the low word passed as the per-channel unpause delta.
        unsigned int newLo = lo + this->minDelay;
        hi += (newLo < lo);
        (void)hi;
        int delta = static_cast<int>(newLo);

        for (int i = 0; i < this->channels.nElem; ++i)
        {
            UnpauseChannel(this->channels[i].current.channel, delta);
            UnpauseChannel(this->channels[i].queued.channel, delta);
        }
    }
}
