#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/hcex/HALO_SOUND_LIST.h"

extern "C" void dlFree(void *ptr); // boundary -- dlmalloc allocator free

// dsVECTOR<HALO_SOUND_LIST,8>::~dsVECTOR @ 0x836B70E8
// Destroy each live list element (the compiler inlined ~HALO_SOUND_LIST: free the three dsVECTOR
// backing stores, then drop levelName/fileName's shared-string references, freeing each buffer
// only on the last reference), then free this vector's own element storage.
template<>
dsVECTOR<HALO_SOUND_LIST, 8>::~dsVECTOR()
{
    for (int index = 0; index < this->nElem; ++index) {
        HALO_SOUND_LIST *list = &this->pData[index];
        dlFree(list->streamUsed.pData);
        dlFree(list->streams.pData);
        dlFree(list->tags.pData);

        dsTSTRING_BUF_HEADER<char> *fileNameBuffer = list->fileName.pBuffer;
        if (fileNameBuffer->refCount-- == 1)
            dlFree(fileNameBuffer);

        dsTSTRING_BUF_HEADER<char> *levelNameBuffer = list->levelName.pBuffer;
        if (levelNameBuffer->refCount-- == 1)
            dlFree(levelNameBuffer);
    }
    dlFree(this->pData);
}
