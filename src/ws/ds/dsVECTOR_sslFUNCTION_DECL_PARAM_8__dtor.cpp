#include "dsVECTOR.h"
#include "../ssl/sslFUNCTION_DECL.h"
#include "../ds/dsDATA_TYPE.h"

// Debug heap free; body not reversed here.
extern "C" void dlFree(void *ptr); // boundary

// dsVECTOR<sslFUNCTION_DECL::PARAM,8>::~dsVECTOR @ 0x82A42FA0
// Destroy each live element's default-value payload (dispatching the stored dsDATA_TYPE::Destroy),
// then release the backing storage. The element PARAM is otherwise trivially destructible.
template<>
dsVECTOR<sslFUNCTION_DECL::PARAM, 8>::~dsVECTOR()
{
    for (int i = 0; i < this->nElem; ++i)
    {
        sslFUNCTION_DECL::PARAM &param = this->pData[i];
        if (param.defValue.type)
            param.defValue.type->Destroy(&param.defValue.storage);
    }
    dlFree(this->pData);
}
