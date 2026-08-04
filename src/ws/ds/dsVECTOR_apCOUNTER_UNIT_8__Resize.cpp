#include <new>
#include "dsVECTOR.h"
#include "ds_assert_boundary.h"
#include "../ap/apCOUNTER_UNIT.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<apCOUNTER_UNIT,8>::Resize @ 0x82719BE0
// Resize the live count to `size`. Growing past capacity reallocates, then default-constructs the
// newly exposed counters (empty name). Shrinking destroys the dropped counters, which reduces to
// releasing each one's ref-counted `name` buffer.
template<>
void dsVECTOR<apCOUNTER_UNIT, 8>::Resize(int size)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            211,
            "size",
            size);

    int nElem = this->nElem;
    if (size >= nElem) {
        if (size > nElem) {
            if (size > this->allocated)
                this->Realloc(size);
            for (int index = this->nElem; index < size; ++index)
                ::new ((void *)&this->pData[index]) apCOUNTER_UNIT(dsStrongAssertMessage);
        }
        this->nElem = size;
    } else {
        for (int index = size; index < this->nElem; ++index) {
            dsTSTRING_BUF_HEADER<char> *buffer = this->pData[index].name.pBuffer;
            if (buffer->refCount-- == 1)
                dlFree(buffer);
        }
        this->nElem = size;
    }
}
