#include "../headers/hcex/HALO_SOUND_LIST_boundary.h"

// 0x836BD2E8 — ?ReleaseSound@HALO_SOUND_LIST@@QAAXPAVSound@FMOD@@@Z
// Mark the stream backing `sound` free again (streaming case). isMemory only strong-asserts
// streams[0] == sound (the one resident bank is never released here). Off-thread => log + assert.
void HALO_SOUND_LIST::ReleaseSound(FMOD::Sound *sound)
{
    if ((snd::System->state.val & 1) == 0 || snd::THREAD_ID == osGetCurThreadId()) {
        if (this->isMemory) {
            if (!IGNORE_STRONG_ASSERT) {
                if (!this->isLoaded)
                    STRONG_ASSERT_DUMMY::Crash(nullptr, "isLoaded",
                                               "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                                               1619, empty_string);
                if (this->streams[0] != sound)
                    STRONG_ASSERT_DUMMY::Crash(nullptr, "streams[0] == sound",
                                               "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                                               1620, empty_string);
            }
        } else {
            int i = 0;
            while (i < this->streams.nElem && this->streams[i] != sound)
                ++i;
            if (i >= this->streams.nElem) {
                if (!IGNORE_STRONG_ASSERT)
                    STRONG_ASSERT_DUMMY::Crash(nullptr, "false",
                                               "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                                               1633, empty_string);
                return;
            }
            if (!IGNORE_STRONG_ASSERT && !this->streamUsed[i])
                STRONG_ASSERT_DUMMY::Crash(nullptr, "streamUsed[i]",
                                           "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                                           1627, empty_string);
            this->streamUsed[i] = false;
        }
    } else {
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
                            "HALO_SOUND_LIST::ReleaseSound", snd::THREAD_ID, osGetCurThreadId());
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != osGetCurThreadId())
            STRONG_ASSERT_DUMMY::Crash(nullptr, "snd::THREAD_ID == osGetCurThreadId()",
                                       "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                                       1616, empty_string);
    }
}
