#include "dsVECTOR.h"
#include "../ssl/sslSCRIPTED_FUNCTION_BASE.h"

extern "C" void dlFree(void *ptr); // boundary

// dsVECTOR<sslSCRIPTED_FUNCTION_BASE::LOCAL_VAR,8>::~dsVECTOR @ 0x825B9E38
// Destroy every live local-variable record — each owns a ref-counted dsTSTRING name whose buffer is
// released (freed at zero) — then free the backing storage. The remaining fields (typeId/declarePos/
// stackPos) are trivial, so no further per-element teardown is needed.
template<>
dsVECTOR<sslSCRIPTED_FUNCTION_BASE::LOCAL_VAR, 8>::~dsVECTOR()
{
    for (int index = 0; index < this->nElem; ++index)
    {
        dsTSTRING_BUF_HEADER<char> *buffer = this->pData[index].name.pBuffer;
        if (--buffer->refCount == 0)
            dlFree(buffer);
    }
    dlFree(this->pData);
}
