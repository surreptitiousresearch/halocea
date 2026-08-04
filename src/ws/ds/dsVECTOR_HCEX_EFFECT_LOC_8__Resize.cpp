#include "dsVECTOR.h"
#include "ds_assert_boundary.h"
#include "../../headers/hcex/HCEX_EFFECT_LOC.h"
#include <new>

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<HCEX_EFFECT_LOC,8>::Resize @ 0x823E24C0
// Resize the live count to `size`. Each HCEX_EFFECT_LOC's only non-trivial member is its `name`
// (dsTSTRING<char>). When growing, newly exposed slots are default-constructed (empty name);
// when shrinking, the elements above `size` are destroyed (the inlined dsTSTRING destructor drops
// one reference on the shared buffer header).
template<>
void dsVECTOR<HCEX_EFFECT_LOC, 8>::Resize(int size)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            211,
            "size",
            size);
    if (size >= this->nElem) {
        if (size > this->nElem) {
            if (size > this->allocated)
                this->Realloc(size);
            for (int growIndex = this->nElem; growIndex < size; ++growIndex)
                if (&this->pData[growIndex])
                    new (&this->pData[growIndex]) HCEX_EFFECT_LOC();
        }
        this->nElem = size;
    } else {
        for (int shrinkIndex = size; shrinkIndex < this->nElem; ++shrinkIndex) {
            dsTSTRING_BUF_HEADER<char> *nameBuffer = this->pData[shrinkIndex].name.pBuffer;
            if (nameBuffer->refCount-- == 1) // last reference: release the buffer
                dlFree(nameBuffer);
        }
        this->nElem = size;
    }
}
