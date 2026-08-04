#include "dsVECTOR.h"
#include "dsTSTRING.h"
#include "ds_assert_boundary.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<dsTSTRING<char>,8>::ShrinkResize @ 0x823B5650
// Shrink the live count to `size`, destroying every string element from `size` up to the old
// count. The inlined ~dsTSTRING<char> drops one reference on the shared buffer header, freeing
// it when the last reference goes away.
template<>
void dsVECTOR<dsTSTRING<char>, 8>::ShrinkResize(int size)
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
            dsTSTRING_BUF_HEADER<char> *buffer = this->pData[i].pBuffer;
            if (buffer->refCount-- == 1) // last reference: release the buffer
                dlFree(buffer);
        }
        this->nElem = size;
    }
}
