#include "dsVECTOR.h"
#include "dsAFFIX_BUILDER.h"
#include "dsTSTRING.h"
#include "ds_assert_boundary.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<dsAFFIX_BUILDER::AFFIX,8>::ShrinkResize @ 0x82A762F0
// Shrink the live count to `size`, destroying every AFFIX from `size` up to the old count. The
// inlined ~AFFIX first destroys the affix's `params` vector (~dsVECTOR<AFF_PARAM,8>), then drops
// one reference on the affix's `name` buffer, freeing it when the count reaches zero.
template<>
void dsVECTOR<dsAFFIX_BUILDER::AFFIX, 8>::ShrinkResize(int size)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            270,
            "size", size);
    if (size < this->nElem) {
        for (int i = size; i < this->nElem; ++i) {
            dsAFFIX_BUILDER::AFFIX *entry = &this->pData[i];
            entry->params.~dsVECTOR();                       // inlined ~AFFIX: destroy parameter vector
            dsTSTRING_BUF_HEADER<char> *buffer = entry->name.pBuffer;
            if (buffer->refCount-- == 1)                     // last reference: release the name buffer
                dlFree(buffer);
        }
        this->nElem = size;
    }
}
