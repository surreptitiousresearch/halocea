#include "../../headers/ws/ai/aiNAV_EDGE_DATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"

extern "C" void dlFree(void *ptr); // engine heap free (boundary)

// aiNAV_EDGE_DATA::_RemoveData() @ 0x8318AD98
// Release the optional per-edge custom-data block (releasing its embedded string buffer, which the
// compiler inlined as a manual refcount decrement) and null the pointer.
void aiNAV_EDGE_DATA::_RemoveData()
{
    aiNAV_EDGE_CUSTOM_DATA *customData = this->customData;
    if (customData)
    {
        dsTSTRING_BUF_HEADER<char> *pBuffer = customData->smartObjName.pBuffer;
        if (--pBuffer->refCount == 0)
            dlFree(pBuffer);
        ::operator delete(customData);
        this->customData = nullptr;
    }
}
