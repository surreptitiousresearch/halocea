#include "../headers/hcex/HALO_PERM_SOUND.h"

// HALO_PERM_SOUND::HALO_PERM_SOUND() @ 0x836B4F68
// Field-by-field init of an unbound playback slot (fsbIndex/identifier = -1 mark it invalid).
HALO_PERM_SOUND::HALO_PERM_SOUND()
{
    this->info = nullptr;
    this->soundList = nullptr;
    this->fsbIndex = -1;
    this->previousFreq = 0.0f;
    this->soundBank = nullptr;
    this->subSound = nullptr;
    this->channel = nullptr;
    this->previousChannel = nullptr;
    this->initTime = 0;
    this->isUsed = false;
    this->wasUsed = false;
    this->prevFadeOut = nullptr;
    this->fadeIn = nullptr;
    this->params = nullptr;
    this->tagInfo = nullptr;
    this->identifier = -1;
}
