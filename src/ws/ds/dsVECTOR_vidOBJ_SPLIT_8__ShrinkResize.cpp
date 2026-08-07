#include "dsVECTOR.h"
#include "../vid/vidOBJ_SPLIT.h"
#include "ds_assert_boundary.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<vidOBJ_SPLIT,8>::ShrinkResize @ 0x82C870D8
// Shrink the live count to `size`, destroying every element from `size` up to the old count:
// free the mtlList's backing array directly (NOT via mtlList's own destructor — a raw dlFree of
// the pointer, faithfully reproduced) then destroy the objDesc.mtl D3D material-state block.
template<>
void dsVECTOR<vidOBJ_SPLIT, 8>::ShrinkResize(int size)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            270,
            "size",
            size);
    if (size < this->nElem) {
        for (int i = size; i < this->nElem; ++i) {
            vidOBJ_SPLIT *elem = &this->pData[i];
            dlFree(elem->mtlList.pData);
            elem->objDesc.mtl.~vidPASS_OBJ_MTL(); // DB vidPASS_OBJ::MTL::~MTL (flattened spelling)
        }
        this->nElem = size;
    }
}
