#include "../headers/hcex/HALO_SOUND_LIST_boundary.h"

// 0x836C1F48 — ?GetFreeSound@HALO_SOUND_LIST@@QAAPAVSound@FMOD@@_N@Z
// Hand a channel a playable FMOD::Sound. isMemory always returns the one resident bank (streams[0]).
// Streaming: scan for an idle stream (streamUsed == false), preferring one that has finished opening
// (FMOD_OPENSTATE_READY); mark it used and return it. If none is idle, try to reclaim one via
// HALO_SOUND_SYSTEM::FindUnused (when tryToUsePreloaded) and retry; otherwise open a brand-new
// stream, append it (marked used), and return it — logging an "EXTEND" growth line.
//
// DEVIATION: decompiler-invented spill args (a3..a5) dropped; the real signature is (bool).
FMOD::Sound *HALO_SOUND_LIST::GetFreeSound(bool tryToUsePreloaded)
{
    if ((snd::System->state.val & 1) != 0 && snd::THREAD_ID != osGetCurThreadId()) {
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
                            "HALO_SOUND_LIST::GetFreeSound", snd::THREAD_ID, osGetCurThreadId());
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != osGetCurThreadId())
            STRONG_ASSERT_DUMMY::Crash(nullptr, "snd::THREAD_ID == osGetCurThreadId()",
                                       "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                                       1463, empty_string);
        return nullptr;
    }

    if (!IGNORE_STRONG_ASSERT && !this->isLoaded)
        STRONG_ASSERT_DUMMY::Crash(nullptr, "isLoaded",
                                   "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                                   1465, empty_string);

    if (this->isMemory)
        return this->streams[0];

    int candidate = -1;
    for (int i = 0; i < this->streams.nElem; ++i) {
        if (!this->streamUsed[i]) {
            FMOD_OPENSTATE state;
            this->streams[i]->getOpenState(&state, nullptr, nullptr);
            if (candidate == -1 || state == FMOD_OPENSTATE_READY) {
                candidate = i;
                if (state == FMOD_OPENSTATE_READY)
                    break;
            }
        }
    }

    if (candidate != -1) {
        this->streamUsed[candidate] = true;
        return this->streams[candidate];
    }

    if (tryToUsePreloaded && haloSoundSystem->FindUnused(this, true) != -1)
        return this->GetFreeSound(false); // a stream was reclaimed; retry the idle scan

    // Grow the stream pool by one freshly-opened stream.
    FMOD::Sound *sound = this->CreateSound();
    this->streams.PushBack(sound);
    bool used = true;
    this->streamUsed.PushBack(used);

    haloSoundSystem->PrintUsedSounds("[DBG] ");
    dsTSTRING<char> msg;
    dsSPrintf(&msg, "EXTEND %d %s\n", this->streams.nElem, this->fileName.pBuffer->str);
    if (dbg_printBufPlay_10.value)
        osOutputDebugString("%s", msg.pBuffer->str);

    return this->streams.Back();
}
