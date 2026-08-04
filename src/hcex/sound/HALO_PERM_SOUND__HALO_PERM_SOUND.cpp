// ??0HALO_PERM_SOUND@@QAA@XZ -- 0x836B4F74
#include "../../headers/hcex/HALO_PERM_SOUND.h"

HALO_PERM_SOUND::HALO_PERM_SOUND()
{
    info = nullptr;
    soundList = nullptr;
    fsbIndex = -1;
    previousFreq = 0.0f;
    soundBank = nullptr;
    subSound = nullptr;
    channel = nullptr;
    previousChannel = nullptr;
    initTime = 0;
    isUsed = false;
    wasUsed = false;
    prevFadeOut = nullptr;
    fadeIn = nullptr;
    params = nullptr;
    tagInfo = nullptr;
    identifier = -1;
}
