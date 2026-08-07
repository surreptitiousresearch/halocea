#include <new>
#include "dsVECTOR.h"
#include "../vid/vidOBJ_SPLIT.h"
#include "ds_assert_boundary.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<vidOBJ_SPLIT,8>::Resize(int) @ 0x82C875F0
// Resize the live count to `size`: shrinking destroys every dropped element's mtlList array and
// objDesc.mtl block (same cleanup as ShrinkResize); growing reallocates if needed then
// default-constructs each newly exposed element.
template<>
void dsVECTOR<vidOBJ_SPLIT, 8>::Resize(int size)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            211,
            "size",
            size);

    int nElem = this->nElem;
    if (size >= nElem) {
        if (size > nElem) {
            if (size > this->allocated)
                this->Realloc(size);
            for (int index = this->nElem; index < size; ++index)
                new (&this->pData[index]) vidOBJ_SPLIT();
        }
        this->nElem = size;
    } else {
        for (int index = size; index < this->nElem; ++index) {
            vidOBJ_SPLIT *elem = &this->pData[index];
            dlFree(elem->mtlList.pData);
            elem->objDesc.mtl.~vidPASS_OBJ_MTL(); // DB vidPASS_OBJ::MTL::~MTL (flattened spelling)
        }
        this->nElem = size;
    }
}
