#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "dsTSTRING.h"
#include "ds_assert_boundary.h"
#include "../../headers/hcex/HCEX_MODEL.h" // reuse the real HCEX_MODEL layout

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<dsPAIR<unsigned long,HCEX_MODEL>,8>::ShrinkResize @ 0x823DC178
// Shrink the live count to `size`, destroying each pair above the new count. The unsigned-long key
// is trivial; the HCEX_MODEL value owns containers and two ref-counted name buffers, so its
// (inlined) destructor is run per element: tear down regions and objNames, free the nodeIndices
// storage, then drop a reference on the hcexName and haloName buffers (freeing at zero).
template<>
void dsVECTOR<dsPAIR<unsigned long, HCEX_MODEL>, 8>::ShrinkResize(int size)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            270,
            "size", size);

    if (size < this->nElem) {
        for (int index = size; index < this->nElem; ++index) {
            HCEX_MODEL *model = &this->pData[index].val; // inlined ~HCEX_MODEL
            model->regions.~dsVECTOR();
            dlFree(model->nodeIndices.pData);
            model->objNames.~dsVECTOR();

            dsTSTRING_BUF_HEADER<char> *hcexBuf = model->hcexName.pBuffer;
            if (hcexBuf->refCount-- == 1)
                dlFree(hcexBuf);
            dsTSTRING_BUF_HEADER<char> *haloBuf = model->haloName.pBuffer;
            if (haloBuf->refCount-- == 1)
                dlFree(haloBuf);
        }
        this->nElem = size;
    }
}
