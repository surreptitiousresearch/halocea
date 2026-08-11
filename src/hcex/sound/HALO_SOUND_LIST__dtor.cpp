#include "../../headers/hcex/HALO_SOUND_LIST.h"

extern "C" void dlFree(void *ptr); // boundary -- dlmalloc allocator free

// HALO_SOUND_LIST::~HALO_SOUND_LIST() @ 0x836B6D78
// Free the three dsVECTOR backing stores directly (streamUsed/streams/tags hold no owned
// sub-objects needing per-element teardown -- bool/FMOD::Sound*/HALO_SOUND_TAG are all
// trivially destructible here), then drop levelName/fileName's shared-string references,
// freeing the buffer only when this was the last reference.
HALO_SOUND_LIST::~HALO_SOUND_LIST()
{
    dlFree(this->streamUsed.pData);
    dlFree(this->streams.pData);
    dlFree(this->tags.pData);

    dsTSTRING_BUF_HEADER<char> *fileNameBuffer = this->fileName.pBuffer;
    if (fileNameBuffer->refCount-- == 1)
        dlFree(fileNameBuffer);

    dsTSTRING_BUF_HEADER<char> *levelNameBuffer = this->levelName.pBuffer;
    if (levelNameBuffer->refCount-- == 1)
        dlFree(levelNameBuffer);
}
