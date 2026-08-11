#include "../headers/hcex/HALO_CHANNEL_boundary.h"

// HALO_CHANNEL::Play @ 0x836C3BB8 (private) — bind `sound` to this channel: borrow a matching
// still-unused preloadList entry's already-opened FMOD banks if one exists, else allocate a fresh
// bank via HALO_SOUND_LIST::GetFreeSound; carry over `previous`'s FMOD channel for stitching if
// given, then CheckNotReady to actually start playback once the bank is ready.
void HALO_CHANNEL::Play(HALO_PERM_SOUND *sound, HALO_PERM_SOUND *previous)
{
    if ((snd::System->state.val & 1) != 0 && snd::THREAD_ID != osGetCurThreadId())
    {
        osOutputDebugString("%s called from wrong thread, expecting 0x%x, i'm in 0x%x\n",
                            "HALO_CHANNEL::Play", snd::THREAD_ID, osGetCurThreadId());
        if (!IGNORE_STRONG_ASSERT && snd::THREAD_ID != osGetCurThreadId())
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("snd::THREAD_ID == osGetCurThreadId()",
                "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1642, empty_string);
        return;
    }

    if (!IGNORE_STRONG_ASSERT && sound->fsbIndex == -1)
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("sound.fsbIndex != -1",
            "D:\\Projects\\code\\HCEX\\sources\\sound\\fmod\\sound_dsound_fmod.cpp", 1644, empty_string);

    bool borrowedBanks = false;
    for (int i = 0; i < haloSoundSystem->preloadList.nElem; ++i)
    {
        HALO_PERM_SOUND *preloaded = &haloSoundSystem->preloadList[i];
        if (sound->info == preloaded->info)
        {
            if (!preloaded->isUsed)
            {
                borrowedBanks = true;
                sound->soundBank = preloaded->soundBank;
                sound->subSound = preloaded->subSound;
                sound->isUsed = true;
                preloaded->isUsed = true;
                preloaded->wasUsed = true;
            }
            break;
        }
    }

    if (!borrowedBanks)
        sound->soundBank = sound->soundList->GetFreeSound(true);

    if (previous)
        sound->previousChannel = previous->channel;

    CheckNotReady(sound);
}
