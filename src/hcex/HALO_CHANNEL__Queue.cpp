#include "../headers/hcex/HALO_CHANNEL_boundary.h"
#include "../headers/sound_permutation.h"
#include <string.h> // memcpy

// HALO_CHANNEL::Queue @ 0x836C4C88 — queue `perm`/`identifier` to play next: if idle, start it
// immediately via the inlined promotion + Play(); if already playing, park it in `queued`
// (promoting ST_PLAYING -> ST_PLAYING_HAS_QUEUED) for Update() to crossfade into once ready. A
// no-op while ST_CROSSFADE is in progress.
void HALO_CHANNEL::Queue(const sound_permutation *perm, int identifier)
{
    if ((snd::System->state.val & 1) != 0 && snd::THREAD_ID != osGetCurThreadId())
    {
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
                            "HALO_CHANNEL::Queue", snd::THREAD_ID, osGetCurThreadId());
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != osGetCurThreadId())
            STRONG_ASSERT_DUMMY::Crash(nullptr, "snd::THREAD_ID == osGetCurThreadId()",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1685, empty_string);
        return;
    }

    Update();
    if (this->state == ST_CROSSFADE)
        return;

    const char *name = tag_get_name(perm->runtime_tag_index);
    if (snd::SystemFMod->dbg.IsBreakOn(name))
        osDebugBreak();

    this->queued.Init(perm, identifier, this->props);
    if (this->queued.fsbIndex == -1)
        return;

    if (this->state == ST_IDLE)
    {
        if (!IGNORE_STRONG_ASSERT && (this->current.soundBank || this->current.channel))
            STRONG_ASSERT_DUMMY::Crash(nullptr, "current.soundBank == 0 && current.channel == 0",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1706, empty_string);
        memcpy(&this->current, &this->queued, sizeof(this->current));
        this->queued.Reset();
        Play(&this->current, nullptr);
        this->state = ST_PLAYING;
        if (dbg_printQueuePlay.value)
            osOutputDebugString("[DBG] PLAY %d %s : %s\n", this->index,
                                tag_get_name(perm->runtime_tag_index), perm);
    }
    else if (this->state == ST_PLAYING)
    {
        this->state = ST_PLAYING_HAS_QUEUED;
        if (dbg_printQueuePlay.value)
            osOutputDebugString("[DBG] QUEUE %s: %s -> %s\n",
                                tag_get_name(perm->runtime_tag_index),
                                this->current.info, this->queued.info);
    }
}
