#include "../../headers/hcex/HALO_CHANNEL.h"

extern "C" void *memset(void *dst, int value, unsigned int size); // boundary -- CRT

// HALO_CHANNEL::HALO_CHANNEL(int index) @ 0x836B59BC
// Construct an idle channel bound to the given slot `index`; current/queued both start
// invalid (fsbIndex==-1, every FMOD pointer null), props zeroed.
HALO_CHANNEL::HALO_CHANNEL(int index) : index(index)
{
    this->state = ST_IDLE;

    this->current.info = nullptr;
    this->current.previousFreq = 0.0f;
    this->current.soundList = nullptr;
    this->current.fsbIndex = -1;
    this->current.soundBank = nullptr;
    this->current.subSound = nullptr;
    this->current.channel = nullptr;
    this->current.previousChannel = nullptr;
    this->current.initTime = 0;
    this->current.isUsed = false;
    this->current.wasUsed = false;
    this->current.prevFadeOut = nullptr;
    this->current.fadeIn = nullptr;
    this->current.params = nullptr;
    this->current.tagInfo = nullptr;
    this->current.identifier = -1;

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

    memset(&this->props, 0, sizeof(this->props));
}
