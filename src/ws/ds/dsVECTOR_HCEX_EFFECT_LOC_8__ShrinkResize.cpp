#include "dsVECTOR.h"
#include "ds_assert_boundary.h"
#include "../../headers/hcex/HCEX_EFFECT_LOC.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<HCEX_EFFECT_LOC,8>::ShrinkResize @ 0x823D8AB8
// Shrink the live count to `size`. Each HCEX_EFFECT_LOC's only non-trivial member is its `name`
// (dsTSTRING<char>), whose inlined destructor drops one reference on the shared buffer header.
template<>
void dsVECTOR<HCEX_EFFECT_LOC, 8>::ShrinkResize(int size)
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
            dsTSTRING_BUF_HEADER<char> *buffer = this->pData[i].name.pBuffer;
            if (buffer->refCount-- == 1) // last reference: release the buffer
                dlFree(buffer);
        }
        this->nElem = size;
    }
}
