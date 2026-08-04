#include "dsVECTOR.h"

// boundary — ps-subsystem section handle: a single pointer to a ref-counted section body.
struct psSECTION_DATA;
struct psSECTION { psSECTION_DATA *pData; };
extern void psSECTION_DATA_Release(psSECTION_DATA *self); // psSECTION_DATA::Release — boundary
extern "C" void dlFree(void *ptr); // boundary — debug heap free

// dsVECTOR<psSECTION,8>::~dsVECTOR @ 0x825173A8
// Release every live element's section body, then free the backing storage.
template<>
dsVECTOR<psSECTION, 8>::~dsVECTOR()
{
    for (int index = 0; index < this->nElem; ++index) {
        psSECTION_DATA *body = this->pData[index].pData;
        if (body)
            psSECTION_DATA_Release(body);
    }
    dlFree(this->pData);
}
