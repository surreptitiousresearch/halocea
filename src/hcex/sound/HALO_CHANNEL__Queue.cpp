#include <stdint.h>
#include "../../headers/hcex/HALO_CHANNEL.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"
#include "../../headers/hcex/HALO_SOUND_LIST_HALO_CHANNEL_globals.h"
#include "../../headers/sound_permutation.h"

extern "C" unsigned int osGetCurThreadId();               // boundary
extern "C" void         osOutputDebugString(const char *fmt, ...); // boundary
extern "C" char *tag_get_name(int16_t tag_index); // boundary -- src/tag_get_name.c

extern "C" void *memcpy(void *dst, const void *src, unsigned int n); // boundary -- CRT
extern "C" void  osDebugBreak(); // boundary

// HALO_CHANNEL::Queue(const sound_permutation *perm, int identifier) @ 0x836C4CC8
// Advance the state machine first (Update()); a no-op while ST_CROSSFADE is already in
// progress. Initialize `queued` from perm/identifier/props; if it failed to resolve a valid FSB
// sub-sound (fsbIndex==-1), leave it queued-but-invalid (Update will just skip it). If idle,
// play it immediately as `current` (idle means current has no bank/channel yet). If already
// playing, promote ST_PLAYING to ST_PLAYING_HAS_QUEUED so Update() picks it up once ready.
void HALO_CHANNEL::Queue(const sound_permutation *perm, int identifier)
{
    if ((snd::System->state.val & 1) != 0 && snd::THREAD_ID != osGetCurThreadId())
    {
        unsigned int curThreadId = osGetCurThreadId();
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
                             "HALO_CHANNEL::Queue", snd::THREAD_ID, curThreadId);
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != osGetCurThreadId())
            STRONG_ASSERT_DUMMY().Crash(
                "snd::THREAD_ID == osGetCurThreadId()",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1685,
                empty_string);
        return;
    }

    this->Update();
    if (this->state == ST_CROSSFADE)
        return;

    const char *tagName = tag_get_name(perm->runtime_tag_index);
    if (snd::SystemFMod->dbg.IsBreakOn(tagName))
        osDebugBreak();

    this->queued.Init(perm, identifier, this->props);
    if (this->queued.fsbIndex == -1)
        return;

    if (this->state != ST_IDLE)
    {
        if (this->state == ST_PLAYING)
        {
            this->state = ST_PLAYING_HAS_QUEUED;
            if (dbg_printQueuePlay.value)
            {
                const char *queuedName = this->queued.info->name;
                const char *currentName = this->current.info->name;
                const char *permTagName = tag_get_name(perm->runtime_tag_index);
                osOutputDebugString("[DBG] QUEUE %s: %s -> %s\n", permTagName, currentName, queuedName);
            }
        }
        return;
    }

    if (!IGNORE_STRONG_ASSERT && (this->current.soundBank || this->current.channel))
        STRONG_ASSERT_DUMMY().Crash(
            "current.soundBank == 0 && current.channel == 0",
            "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1706,
            empty_string);

    memcpy(&this->current, &this->queued, sizeof(this->current));

    this->queued.previousFreq = 0.0f;
    this->queued.info = nullptr;
    this->queued.soundList = nullptr;
    this->queued.fsbIndex = -1;
    this->queued.soundBank = nullptr;
    this->queued.subSound = nullptr;
    this->queued.channel = nullptr;
    this->queued.previousChannel = nullptr;
    this->queued.initTime = 0;
    this->queued.isUsed = false;
    this->queued.wasUsed = false;
    this->queued.prevFadeOut = nullptr;
    this->queued.fadeIn = nullptr;
    this->queued.params = nullptr;
    this->queued.tagInfo = nullptr;
    this->queued.identifier = -1;

    this->Play(&this->current, nullptr);
    this->state = ST_PLAYING;

    if (dbg_printQueuePlay.value)
    {
        const char *permTagName = tag_get_name(perm->runtime_tag_index);
        osOutputDebugString("[DBG] PLAY %d %s : %s\n", this->index, permTagName, perm->name);
    }
}
