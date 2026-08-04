#include "../headers/hcex/HALO_SOUND_LIST_boundary.h"

// 0x836BCF78 — ?Unload@HALO_SOUND_LIST@@QAAXXZ
// Fully unload an isMemory bank: release its one FMOD::Sound, free the owned `data` buffer, and
// clear the loaded flag. A no-op unless (isLoaded && isMemory) — streaming lists own nothing here.
void HALO_SOUND_LIST::Unload()
{
    if (this->isLoaded && this->isMemory) {
        this->streams[0]->release();
        this->streams.nElem = 0;
        operator delete[](this->data);
        this->data = nullptr;
        this->dataLength = 0;
        this->isLoaded = false;
    }
}
