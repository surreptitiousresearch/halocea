#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "dsTSTRING.h"
#include "ds_assert_boundary.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<dsPAIR<dsTSTRING<char>,int>,8>::ShrinkResize @ 0x823B5D30
// Shrink the live count to `size`, dropping a reference on each element's dsTSTRING key above the
// new count (freeing its buffer at zero). The trailing int value is trivial.
template<>
void dsVECTOR<dsPAIR<dsTSTRING<char>, int>, 8>::ShrinkResize(int size)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            270,
            "size", size);

    if (size < this->nElem) {
        for (int index = size; index < this->nElem; ++index) {
            dsTSTRING_BUF_HEADER<char> *buffer = this->pData[index].key.pBuffer;
            if (buffer->refCount-- == 1)
                dlFree(buffer);
        }
        this->nElem = size;
    }
}
