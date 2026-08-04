#include "dsVECTOR.h"
#include "dsTSTRING.h"
#include "ds_assert_boundary.h"
#include "../../headers/hcex/HCEX_MODEL_REGION.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<HCEX_MODEL_REGION,8>::ShrinkResize @ 0x823D89F8
// Shrink the live count to `size`, destroying each HCEX_MODEL_REGION above the new count. The
// element's destructor is inlined: drop a reference on the region-name buffer (freeing at zero),
// destroy the objNames string vector, then free the objIds backing array.
template<>
void dsVECTOR<HCEX_MODEL_REGION, 8>::ShrinkResize(int size)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            270,
            "size", size);

    if (size < this->nElem) {
        for (int index = size; index < this->nElem; ++index) {
            HCEX_MODEL_REGION *region = &this->pData[index];
            dsTSTRING_BUF_HEADER<char> *nameBuffer = region->name.pBuffer;
            if (nameBuffer->refCount-- == 1)
                dlFree(nameBuffer);
            region->objNames.~dsVECTOR();
            dlFree(region->objIds.pData);
        }
        this->nElem = size;
    }
}
