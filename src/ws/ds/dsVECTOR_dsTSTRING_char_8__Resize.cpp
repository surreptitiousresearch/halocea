#include "dsVECTOR.h"
#include "dsTSTRING.h"
#include "ds_assert_boundary.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<dsTSTRING<char>,8>::Resize @ 0x826885F8
// Grow or shrink the live count to `size`. Growing default-constructs each newly exposed slot
// (adopting the shared empty-string singleton, matching New<dsTSTRING<char>>); shrinking drops a
// buffer reference on every element above the new count first.
template<>
void dsVECTOR<dsTSTRING<char>, 8>::Resize(int size)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            211,
            "size", size);

    int nElem = this->nElem;
    if (size >= nElem) {
        if (size > nElem) {
            if (size > this->allocated)
                this->Realloc(size);
            for (int index = this->nElem; index < size; ++index)
                this->CallNew<dsTSTRING<char>>(&this->pData[index]);
        }
        this->nElem = size;
    } else {
        for (int index = size; index < this->nElem; ++index) {
            dsTSTRING_BUF_HEADER<char> *buffer = this->pData[index].pBuffer;
            if (buffer->refCount-- == 1)
                dlFree(buffer);
        }
        this->nElem = size;
    }
}
