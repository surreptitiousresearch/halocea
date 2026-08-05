#include "../../headers/hcex/HALO_CHANNEL.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"
#include "../../headers/hcex/HALO_SOUND_LIST_HALO_CHANNEL_globals.h"
#include "../../headers/hcex/HALO_SOUND_SYSTEM.h"

extern "C" unsigned int osGetCurThreadId();               // boundary
extern "C" void         osOutputDebugString(const char *fmt, ...); // boundary

// HALO_CHANNEL::Play(HALO_PERM_SOUND *sound, HALO_PERM_SOUND *previous) @ 0x836C3BF8
// Bind `sound` to this channel: first try to borrow a matching, still-checked-out
// haloSoundSystem->preloadList entry's already-open FMOD banks (same `info`, not yet used) so a
// preloaded sound starts instantly; otherwise fetch a free one via
// HALO_SOUND_LIST::GetFreeSound(tryToUsePreloaded=true). If `previous` is given, carry over its
// FMOD channel so CheckNotReady can stitch playback across the loop point. Finally
// CheckNotReady actually starts (or continues waiting to start) playback.
void HALO_CHANNEL::Play(HALO_PERM_SOUND *sound, HALO_PERM_SOUND *previous)
{
    if ((snd::System->state.val & 1) != 0 && snd::THREAD_ID != osGetCurThreadId())
    {
        unsigned int curThreadId = osGetCurThreadId();
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
                             "HALO_CHANNEL::Play", snd::THREAD_ID, curThreadId);
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != osGetCurThreadId())
            STRONG_ASSERT_DUMMY().Crash(
                "snd::THREAD_ID == osGetCurThreadId()",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1642,
                empty_string);
        return;
    }

    if (!IGNORE_STRONG_ASSERT && sound->fsbIndex == -1)
        STRONG_ASSERT_DUMMY().Crash(
            "sound.fsbIndex != -1", "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp",
            1644, empty_string);

    bool borrowedPreload = false;
    for (int i = 0; i < haloSoundSystem->preloadList.nElem; ++i)
    {
        HALO_PERM_SOUND &preload = haloSoundSystem->preloadList[i];
        if (sound->info != preload.info)
            continue;

        if (!preload.isUsed)
        {
            borrowedPreload = true;
            sound->soundBank = preload.soundBank;
            sound->subSound = preload.subSound;
            sound->isUsed = true;
            preload.isUsed = true;
            preload.wasUsed = true;
        }
        break;
    }

    if (!borrowedPreload)
        sound->soundBank = sound->soundList->GetFreeSound(true);

    if (previous)
        sound->previousChannel = previous->channel;

    this->CheckNotReady(sound);
}
