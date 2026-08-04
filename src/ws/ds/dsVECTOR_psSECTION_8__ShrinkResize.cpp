#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

// boundary — ps-subsystem section handle: a single pointer to a ref-counted section body.
struct psSECTION_DATA;
struct psSECTION { psSECTION_DATA *pData; };
extern void psSECTION_DATA_Release(psSECTION_DATA *self); // psSECTION_DATA::Release — boundary

// dsVECTOR<psSECTION,8>::ShrinkResize @ 0x8251AB00
// Shrink the live count to `size`, releasing every section body from `size` up to the old count.
// The inlined ~psSECTION releases the ref-counted section body.
template<>
void dsVECTOR<psSECTION, 8>::ShrinkResize(int size)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            270,
            "size", size);
    if (size < this->nElem) {
        for (int i = size; i < this->nElem; ++i) {
            psSECTION_DATA *body = this->pData[i].pData;
            if (body)
                psSECTION_DATA_Release(body);
        }
        this->nElem = size;
    }
}
