#include "../headers/hcex/HALO_SOUND_LIST_boundary.h"

// 0x836BD000 — ?Term@HALO_SOUND_LIST@@QAAXXZ
// Release every open stream/bank and reset all live counts to zero (map teardown). Must run on
// the audio thread once the sound system is live; otherwise it logs a wrong-thread diagnostic and
// strong-asserts without touching FMOD.
void HALO_SOUND_LIST::Term()
{
    if ((snd::System->state.val & 1) == 0 || snd::THREAD_ID == osGetCurThreadId()) {
        for (int i = 0; i < this->streams.nElem; ++i)
            this->streams[i]->release();
        this->streams.nElem = 0;
        this->streamUsed.nElem = 0;
        this->soundsCount = 0;
        this->tags.nElem = 0;
    } else {
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
                            "HALO_SOUND_LIST::Term", snd::THREAD_ID, osGetCurThreadId());
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != osGetCurThreadId())
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("snd::THREAD_ID == osGetCurThreadId()",
                                       "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                                       1397, empty_string);
    }
}
