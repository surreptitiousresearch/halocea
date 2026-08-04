#include "../../headers/hcex/HALO_SOUND_SYSTEM.h"
#include "../../headers/hcex/HALO_SOUND_SYSTEM_globals.h"
#include "../../headers/ws/m3d/m3dRND_GEN.h"

extern m3dRND_GEN m3dRndGen; // boundary — ws-engine shared RNG instance

// HALO_SOUND_SYSTEM::PlayCustomSound(char const*, char const*) @ 0x836C386C
// Looks `soundDefName` up across every loaded sound-list, picks a random sub-sound from the
// matching tag, checks out an FSB bank for it, and kicks off playback via a fresh
// customSounds entry (driven to completion by UpdateCustomSounds()). `name` is only used for
// the not-found diagnostic message.
void HALO_SOUND_SYSTEM::PlayCustomSound(const char *soundDefName, const char *name)
{
    HALO_CUSTOM_SOUND sound;
    sound.soundList = nullptr;
    sound.soundBank = nullptr;
    sound.fsbIndex = -1;
    sound.subSound = nullptr;
    sound.channel = nullptr;

    HALO_SOUND_LIST *matchedList = nullptr;
    const HALO_SOUND_TAG *tag = nullptr;

    for (int i = 0; i < soundLists.nElem; ++i) {
        tag = soundLists[i].Find(soundDefName);
        if (tag) {
            matchedList = &soundLists[i];
            break;
        }
    }

    if (tag) {
        sound.soundList = matchedList;
        sound.fsbIndex = tag->startIndex + m3dRndGen.RndIntMax(tag->count - 1);
    }

    if (matchedList) {
        sound.soundBank = matchedList->GetFreeSound(true);
        customSounds.PushBack(sound);
        HALO_CUSTOM_SOUND &appended = customSounds.Back();
        UpdateCustomSound(appended);
    } else {
        osOutputDebugString("~Sound,Error~Unable to find sound '%s'", name);
    }
}
