#include "../headers/hcex/HALO_CHANNEL_boundary.h"
#include <string.h> // memcpy

// HALO_CHANNEL::Update @ 0x836C3D70 — poll/advance this channel's state machine: finish opening
// current/queued FMOD banks (CheckNotReady), detect `current` finishing and either advance a
// queued sound, hand off to ST_PLAYING_TO_END, or go idle; promote ST_PLAYING_HAS_QUEUED into an
// actual ST_CROSSFADE once queued is ready.
//
// The inlined per-slot field-zeroing blocks in the raw decompile are HALO_PERM_SOUND::Reset()
// (resets fsbIndex/identifier to -1 and every pointer/counter to 0); reproduced as Reset() calls.
// Control flow (incl. the LABEL_24 join) is reproduced exactly from the decompile.
void HALO_CHANNEL::Update()
{
    if ((snd::System->state.val & 1) != 0 && snd::THREAD_ID != osGetCurThreadId())
    {
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
                            "HALO_CHANNEL::Update", snd::THREAD_ID, osGetCurThreadId());
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != osGetCurThreadId())
            STRONG_ASSERT_DUMMY::Crash(nullptr, "snd::THREAD_ID == osGetCurThreadId()",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1962, empty_string);
        return;
    }

    if (this->state == ST_IDLE)
        return;

    // `current` was never actually bound to a bank: drop back to idle.
    if (!this->current.soundBank)
    {
        this->current.Reset();
        this->state = ST_IDLE;
        return;
    }

    // Still waiting on either slot's FMOD bank/sub-sound to finish opening.
    if (CheckNotReady(&this->current))
        return;
    if (CheckNotReady(&this->queued))
        return;

    bool isCurrentPlaying = false;
    this->current.channel->isPlaying(&isCurrentPlaying);

    if (isCurrentPlaying)
    {
        if (this->state != ST_IDLE)
            goto advance_machine; // LABEL_24
        // else (playing while nominally idle): fall through to the queued-promotion check.
    }
    else
    {
        this->current.Stop();
        if (this->state == ST_CROSSFADE)
        {
            if (!IGNORE_STRONG_ASSERT && this->queued.fsbIndex == -1)
                STRONG_ASSERT_DUMMY::Crash(nullptr, "queued.IsValid()",
                    "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1988, empty_string);
            memcpy(&this->current, &this->queued, sizeof(this->current));
            this->current.ResetFadeIn();
            this->queued.Reset();
            this->state = ST_PLAYING;
            return;
        }
        this->state = ST_IDLE;
    }

    // current finished (or playing-while-idle): promote a waiting queued sound to current.
    if (this->queued.fsbIndex != -1)
    {
        if (!IGNORE_STRONG_ASSERT && this->current.soundBank)
            STRONG_ASSERT_DUMMY::Crash(nullptr, "current.soundBank == 0",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 2003, empty_string);
        memcpy(&this->current, &this->queued, sizeof(this->current));
        this->queued.Reset();
        Play(&this->current, nullptr);
        this->state = ST_PLAYING;
        return;
    }

advance_machine: // LABEL_24
    if (this->state == ST_PLAYING_HAS_QUEUED)
    {
        Play(&this->queued, &this->current);
        this->state = ST_CROSSFADE;
    }
    else if (this->state == ST_PLAYING)
    {
        const HALO_SOUND_PARAMS *params = this->current.params;
        if (params && (((unsigned int)params->state.val >> 3) & 1))
        {
            this->state = ST_PLAYING_TO_END;
            haloSoundSystem->playToEnd.PushBack(*this);
            if (!IGNORE_STRONG_ASSERT && this->queued.fsbIndex != -1)
                STRONG_ASSERT_DUMMY::Crash(nullptr, "!queued.IsValid()",
                    "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 2023, empty_string);
            this->current.Reset();
            this->queued.Reset();
            this->state = ST_IDLE;
        }
    }
}
