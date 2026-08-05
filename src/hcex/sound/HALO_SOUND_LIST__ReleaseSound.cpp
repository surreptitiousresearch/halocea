#include "../../headers/hcex/HALO_SOUND_LIST.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"
#include "../../headers/hcex/HALO_SOUND_LIST_HALO_CHANNEL_globals.h"

extern "C" unsigned int osGetCurThreadId();               // boundary
extern "C" void         osOutputDebugString(const char *fmt, ...); // boundary

// HALO_SOUND_LIST::ReleaseSound(FMOD::Sound *sound) @ 0x836BD324
// isMemory: nothing to release (the one bank stays resident) -- just strong-asserts `sound` is
// indeed streams[0] and that isLoaded. Otherwise: find `sound` among `streams` and clear its
// streamUsed flag (strong-asserting it was actually marked used).
void HALO_SOUND_LIST::ReleaseSound(FMOD::Sound *sound)
{
    if ((snd::System->state.val & 1) != 0 && snd::THREAD_ID != osGetCurThreadId())
    {
        unsigned int curThreadId = osGetCurThreadId();
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
                             "HALO_SOUND_LIST::ReleaseSound", snd::THREAD_ID, curThreadId);
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != osGetCurThreadId())
            STRONG_ASSERT_DUMMY().Crash(
                "snd::THREAD_ID == osGetCurThreadId()",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1616,
                empty_string);
        return;
    }

    if (this->isMemory)
    {
        if (!IGNORE_STRONG_ASSERT)
        {
            if (!this->isLoaded)
                STRONG_ASSERT_DUMMY().Crash(
                    "isLoaded", "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                    1619, empty_string);
            else if (this->streams[0] != sound)
                STRONG_ASSERT_DUMMY().Crash(
                    "streams[0] == sound", "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                    1620, empty_string);
        }
        return;
    }

    int i = 0;
    for (; i < this->streams.nElem; ++i)
    {
        if (this->streams[i] == sound)
            break;
    }
    if (i >= this->streams.nElem)
    {
        if (!IGNORE_STRONG_ASSERT)
            STRONG_ASSERT_DUMMY().Crash(
                "false", "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                1633, empty_string);
        return;
    }

    if (!IGNORE_STRONG_ASSERT && !this->streamUsed[i])
        STRONG_ASSERT_DUMMY().Crash(
            "streamUsed[i]", "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
            1627, empty_string);

    this->streamUsed[i] = false;
}
