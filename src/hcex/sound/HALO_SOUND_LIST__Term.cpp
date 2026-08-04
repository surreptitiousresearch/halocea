#include "../../headers/hcex/HALO_SOUND_LIST.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"
#include "../../headers/hcex/HALO_SOUND_LIST_HALO_CHANNEL_globals.h"

extern "C" unsigned int osGetCurThreadId();               // boundary
extern "C" void         osOutputDebugString(const char *fmt, ...); // boundary

// HALO_SOUND_LIST::Term() @ 0x836BD038
// Release every FMOD::Sound in `streams` (both the isMemory single bank and every streaming
// voice) and reset all three tables' live counts to zero -- used at map/level teardown.
void HALO_SOUND_LIST::Term()
{
    if ((snd::System->state.val & 1) != 0 && snd::THREAD_ID != osGetCurThreadId())
    {
        unsigned int curThreadId = osGetCurThreadId();
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
                             "HALO_SOUND_LIST::Term", snd::THREAD_ID, curThreadId);
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != osGetCurThreadId())
            STRONG_ASSERT_DUMMY().Crash(
                "snd::THREAD_ID == osGetCurThreadId()",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1397,
                dsStrongAssertMessage);
        return;
    }

    for (int i = 0; i < this->streams.nElem; ++i)
        this->streams[i]->release();

    this->streams.nElem = 0;
    this->streamUsed.nElem = 0;
    this->soundsCount = 0;
    this->tags.nElem = 0;
}
