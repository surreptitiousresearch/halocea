#include "../../headers/hcex/HALO_CHANNEL.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"
#include "../../headers/hcex/HALO_SOUND_LIST_HALO_CHANNEL_globals.h"
#include "../../headers/hcex/HALO_SOUND_SYSTEM.h"
#include "../../headers/hcex/HALO_SOUND_PARAMS.h"

extern "C" unsigned int osGetCurThreadId();               // boundary
extern "C" void         osOutputDebugString(const char *fmt, ...); // boundary

extern "C" void *memcpy(void *dst, const void *src, unsigned int n); // boundary -- CRT

// HALO_CHANNEL::Update() @ 0x836C3DA8
// Advance this channel's state machine one tick. See HALO_CHANNEL.h for the summary; this is
// the full reconstruction of every transition CheckNotReady/isPlaying/PLAY_TO_END drives.
void HALO_CHANNEL::Update()
{
    if ((snd::System->state.val & 1) != 0 && snd::THREAD_ID != osGetCurThreadId())
    {
        unsigned int curThreadId = osGetCurThreadId();
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
                             "HALO_CHANNEL::Update", snd::THREAD_ID, curThreadId);
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != osGetCurThreadId())
            STRONG_ASSERT_DUMMY().Crash(
                "snd::THREAD_ID == osGetCurThreadId()",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1962,
                dsStrongAssertMessage);
        return;
    }

    if (this->state == ST_IDLE)
        return;

    if (!this->current.soundBank)
    {
        // Corner case: current became invalid without going through the normal stop path.
        this->current.soundBank = nullptr;
        this->current.fadeIn = nullptr;
        this->current.prevFadeOut = nullptr;
        this->current.previousFreq = 0.0f;
        this->current.info = nullptr;
        this->current.soundList = nullptr;
        this->current.fsbIndex = -1;
        this->current.subSound = nullptr;
        this->current.channel = nullptr;
        this->current.previousChannel = nullptr;
        this->current.initTime = 0;
        this->current.isUsed = false;
        this->current.wasUsed = false;
        this->current.params = nullptr;
        this->current.tagInfo = nullptr;
        this->current.identifier = -1;
        this->state = ST_IDLE;
        return;
    }

    if (this->CheckNotReady(&this->current))
        return;
    if (this->CheckNotReady(&this->queued))
        return;

    bool isPlaying = false;
    this->current.channel->isPlaying(&isPlaying);

    if (isPlaying)
    {
        if (this->state == ST_PLAYING_HAS_QUEUED)
        {
            this->Play(&this->queued, &this->current);
            this->state = ST_CROSSFADE;
        }
        else if (this->state == ST_PLAYING)
        {
            const HALO_SOUND_PARAMS *params = this->current.params;
            bool playToEnd = params && ((static_cast<unsigned int>(params->state.val) >> 3) & 1);
            if (playToEnd)
            {
                this->state = ST_PLAYING_TO_END;
                haloSoundSystem->playToEnd.PushBack(*this);

                if (!IGNORE_STRONG_ASSERT && this->queued.fsbIndex != -1)
                    STRONG_ASSERT_DUMMY().Crash(
                        "!queued.IsValid()", "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                        2023, dsStrongAssertMessage);

                this->current.soundBank = nullptr;
                this->current.fadeIn = nullptr;
                this->current.prevFadeOut = nullptr;
                this->current.info = nullptr;
                this->current.soundList = nullptr;
                this->current.fsbIndex = -1;
                this->current.previousFreq = 0.0f;
                this->current.subSound = nullptr;
                this->current.channel = nullptr;
                this->current.previousChannel = nullptr;
                this->current.initTime = 0;
                this->current.isUsed = false;
                this->current.wasUsed = false;
                this->current.params = nullptr;
                this->current.tagInfo = nullptr;
                this->current.identifier = -1;

                this->queued.previousFreq = 0.0f;
                this->queued.soundBank = nullptr;
                this->queued.fadeIn = nullptr;
                this->queued.prevFadeOut = nullptr;
                this->queued.info = nullptr;
                this->queued.soundList = nullptr;
                this->queued.fsbIndex = -1;
                this->queued.subSound = nullptr;
                this->queued.channel = nullptr;
                this->queued.previousChannel = nullptr;
                this->queued.initTime = 0;
                this->queued.isUsed = false;
                this->queued.wasUsed = false;
                this->queued.params = nullptr;
                this->queued.tagInfo = nullptr;
                this->queued.identifier = -1;

                this->state = ST_IDLE;
            }
        }
        return;
    }

    // current finished playing.
    this->current.Stop();
    if (this->state == ST_CROSSFADE)
    {
        if (!IGNORE_STRONG_ASSERT && this->queued.fsbIndex == -1)
            STRONG_ASSERT_DUMMY().Crash(
                "queued.IsValid()", "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                1988, dsStrongAssertMessage);

        memcpy(&this->current, &this->queued, sizeof(this->current));
        this->current.ResetFadeIn();

        this->queued.soundBank = nullptr;
        this->queued.fadeIn = nullptr;
        this->queued.prevFadeOut = nullptr;
        this->queued.info = nullptr;
        this->queued.soundList = nullptr;
        this->queued.previousFreq = 0.0f;
        this->queued.fsbIndex = -1;
        this->queued.subSound = nullptr;
        this->queued.channel = nullptr;
        this->queued.previousChannel = nullptr;
        this->queued.initTime = 0;
        this->queued.isUsed = false;
        this->queued.wasUsed = false;
        this->queued.params = nullptr;
        this->queued.tagInfo = nullptr;
        this->queued.identifier = -1;

        this->state = ST_PLAYING;
        return;
    }
    this->state = ST_IDLE;

    if (this->queued.fsbIndex != -1)
    {
        if (!IGNORE_STRONG_ASSERT && this->current.soundBank)
            STRONG_ASSERT_DUMMY().Crash(
                "current.soundBank == 0", "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
                2003, dsStrongAssertMessage);

        memcpy(&this->current, &this->queued, sizeof(this->current));

        this->queued.soundBank = nullptr;
        this->queued.fadeIn = nullptr;
        this->queued.prevFadeOut = nullptr;
        this->queued.info = nullptr;
        this->queued.soundList = nullptr;
        this->queued.previousFreq = 0.0f;
        this->queued.fsbIndex = -1;
        this->queued.subSound = nullptr;
        this->queued.channel = nullptr;
        this->queued.previousChannel = nullptr;
        this->queued.initTime = 0;
        this->queued.isUsed = false;
        this->queued.wasUsed = false;
        this->queued.params = nullptr;
        this->queued.tagInfo = nullptr;
        this->queued.identifier = -1;

        this->Play(&this->current, nullptr);
        this->state = ST_PLAYING;
    }
    // else: state is already ST_IDLE and there's nothing queued -- nothing left to do
    // (matches the decompile's `goto LABEL_24` re-entry, which is a no-op once state==ST_IDLE).
}
