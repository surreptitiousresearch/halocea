#include "dsVECTOR.h"

// boundary — ps-subsystem section handle: a single pointer to a ref-counted section body.
struct psSECTION_DATA;
struct psSECTION { psSECTION_DATA *pData; };
extern void psSECTION_DATA_Release(psSECTION_DATA *self); // psSECTION_DATA::Release — boundary

// dsVECTOR<psSECTION,8>::Clear @ 0x82516538
// Release every live section body, then reset the element count to zero.
template<>
void dsVECTOR<psSECTION, 8>::Clear()
{
    for (int index = 0; index < this->nElem; ++index) {
        psSECTION_DATA *body = this->pData[index].pData;
        if (body)
            psSECTION_DATA_Release(body);
    }
    this->nElem = 0;
}
