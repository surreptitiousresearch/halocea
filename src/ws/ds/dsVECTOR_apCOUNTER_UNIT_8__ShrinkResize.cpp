#include "dsVECTOR.h"
#include "ds_assert_boundary.h"
#include "../ap/apCOUNTER_UNIT.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<apCOUNTER_UNIT,8>::ShrinkResize @ 0x82714BF8
// Shrink the live count to `size`. Each apCOUNTER_UNIT's only reference-counted member is its
// `name` (dsTSTRING<char>), whose inlined destructor drops one reference on the shared buffer.
template<>
void dsVECTOR<apCOUNTER_UNIT, 8>::ShrinkResize(int size)
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
