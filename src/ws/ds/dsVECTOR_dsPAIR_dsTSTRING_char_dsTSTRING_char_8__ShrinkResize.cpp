#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "dsTSTRING.h"
#include "ds_assert_boundary.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<dsPAIR<dsTSTRING<char>,dsTSTRING<char>>,8>::ShrinkResize @ 0x823D8D58
// Shrink the live count to `size`, dropping a reference on each element's key and val string
// buffers above the new count (value released before key, matching the decompiled order).
template<>
void dsVECTOR<dsPAIR<dsTSTRING<char>, dsTSTRING<char> >, 8>::ShrinkResize(int size)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            270,
            "size", size);

    if (size < this->nElem) {
        for (int index = size; index < this->nElem; ++index) {
            dsPAIR<dsTSTRING<char>, dsTSTRING<char> > *pair = &this->pData[index];
            dsTSTRING_BUF_HEADER<char> *valBuffer = pair->val.pBuffer;
            if (valBuffer->refCount-- == 1)
                dlFree(valBuffer);
            dsTSTRING_BUF_HEADER<char> *keyBuffer = pair->key.pBuffer;
            if (keyBuffer->refCount-- == 1)
                dlFree(keyBuffer);
        }
        this->nElem = size;
    }
}
