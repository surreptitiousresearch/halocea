#include "../../headers/hcex/HALO_SOUND_LIST.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"
#include "../../headers/hcex/HALO_SOUND_LIST_HALO_CHANNEL_globals.h"
#include "../../headers/hcex/HALO_SOUND_SYSTEM.h"

extern "C" unsigned int osGetCurThreadId();               // boundary
extern "C" void         osOutputDebugString(const char *fmt, ...); // boundary


// HALO_SOUND_LIST::GetFreeSound(bool tryToUsePreloaded) @ 0x836C1F64
// isMemory: there is exactly one bank (streams[0]), always returned. Otherwise: scan
// streamUsed for the first idle stream, preferring one that is already fully open
// (getOpenState's `open` out-param false) over one still loading; mark it used and return it.
// If every stream is busy: try to reclaim a still-loading preload-list entry first
// (HALO_SOUND_SYSTEM::FindUnused) when tryToUsePreloaded, else recurse with
// tryToUsePreloaded=false to fall through to opening a brand-new stream, which is appended to
// streams/streamUsed and logged ("EXTEND N file\n" when the debug print toggle is on).
FMOD::Sound *HALO_SOUND_LIST::GetFreeSound(bool tryToUsePreloaded)
{
    if ((snd::System->state.val & 1) != 0 && snd::THREAD_ID != osGetCurThreadId())
    {
        unsigned int curThreadId = osGetCurThreadId();
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
                             "HALO_SOUND_LIST::GetFreeSound", snd::THREAD_ID, curThreadId);
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != osGetCurThreadId())
            STRONG_ASSERT_DUMMY().Crash(
                "snd::THREAD_ID == osGetCurThreadId()",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1463,
                empty_string);
        return nullptr;
    }

    if (!IGNORE_STRONG_ASSERT && !this->isLoaded)
        STRONG_ASSERT_DUMMY().Crash(
            "isLoaded", "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1465,
            empty_string);

    if (this->isMemory)
        return this->streams[0];

    int freeIndex = -1;
    for (int i = 0; i < this->streams.nElem; ++i)
    {
        if (!this->streamUsed[i])
        {
            FMOD_OPENSTATE openState = FMOD_OPENSTATE_READY;
            this->streams[i]->getOpenState(&openState, nullptr, nullptr);
            if (freeIndex == -1 || openState == FMOD_OPENSTATE_READY)
            {
                freeIndex = i;
                if (openState == FMOD_OPENSTATE_READY)
                    break;
            }
        }
    }

    if (freeIndex != -1)
    {
        this->streamUsed[freeIndex] = true;
        return this->streams[freeIndex];
    }

    if (tryToUsePreloaded && haloSoundSystem->FindUnused(this, true) != -1)
        return this->GetFreeSound(false);

    FMOD::Sound *sound = this->CreateSound();
    this->streams.PushBack(sound);
    this->streamUsed.PushBack(true);
    haloSoundSystem->PrintUsedSounds("[DBG] ");

    if (dbg_printBufPlay_10.value)
    {
        dsTSTRING<char> line;
        dsSPrintf(&line, "EXTEND %d %s\n", this->streams.nElem, this->fileName.pBuffer->str);
        osOutputDebugString("%s", line.pBuffer->str);
    }

    return this->streams.Back();
}
