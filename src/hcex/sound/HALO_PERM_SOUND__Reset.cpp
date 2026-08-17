/* ?Reset@HALO_PERM_SOUND@@QAAXXZ @0x836B68B0 */
// ?Reset@HALO_PERM_SOUND@@QAAXXZ -- 0x836B68BC
#include "../../headers/hcex/HALO_PERM_SOUND.h"

void HALO_PERM_SOUND::Reset()
{
    soundBank = nullptr;
    fadeIn = nullptr;
    prevFadeOut = nullptr;
    previousFreq = 0.0f;
    info = nullptr;
    soundList = nullptr;
    fsbIndex = -1;
    subSound = nullptr;
    channel = nullptr;
    previousChannel = nullptr;
    initTime = 0;
    isUsed = false;
    wasUsed = false;
    params = nullptr;
    tagInfo = nullptr;
    identifier = -1;
}
