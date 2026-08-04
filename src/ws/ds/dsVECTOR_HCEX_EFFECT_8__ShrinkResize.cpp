#include "dsVECTOR.h"
#include "ds_assert_boundary.h"
#include "../../headers/hcex/HCEX_EFFECT.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<HCEX_EFFECT,8>::ShrinkResize @ 0x823D8CA0
// Shrink the live count to `size`, destroying each HCEX_EFFECT above it. Every element has two
// non-trivial members: `hcexName` (a dsTSTRING whose inlined destructor drops one reference on the
// shared buffer header, freeing it at zero) and `locations` (a dsVECTOR whose destructor releases
// its own backing storage).
template<>
void dsVECTOR<HCEX_EFFECT, 8>::ShrinkResize(int size)
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
            HCEX_EFFECT *effect = &this->pData[i];
            dsTSTRING_BUF_HEADER<char> *buffer = effect->hcexName.pBuffer;
            if (buffer->refCount-- == 1) // last reference: release the buffer
                dlFree(buffer);
            effect->locations.~dsVECTOR(); // boundary — release the locations backing storage
        }
        this->nElem = size;
    }
}
