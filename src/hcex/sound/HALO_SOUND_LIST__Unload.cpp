#include "../../headers/hcex/HALO_SOUND_LIST.h"

// HALO_SOUND_LIST::Unload() @ 0x836BCF94
// isMemory only, and only while loaded: release the one FMOD::Sound, drop the stream count to
// zero, and free the raw file buffer.
void HALO_SOUND_LIST::Unload()
{
    if (this->isLoaded && this->isMemory)
    {
        this->streams[0]->release();
        this->streams.nElem = 0;
        delete[] this->data;
        this->data = nullptr;
        this->dataLength = 0;
        this->isLoaded = false;
    }
}
