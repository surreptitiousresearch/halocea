#include "dsVECTOR.h"
#include "dsAFFIX_BUILDER.h"
#include "ds_assert_boundary.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<dsAFFIX_BUILDER::AFF_PARAM,8>::ShrinkResize @ 0x82A76230
// Shrink the live count to `size`, destroying each {name,value} pair above the new count. The
// pair's destructor is inlined: drop a reference on the value buffer, then on the name buffer,
// freeing each at zero.
// Note: DB scopes the element as dsAFFIX_BUILDER::AFF_PARAM; the existing header models it as
// dsAFFIX_BUILDER::AFF_PARAM, so we reference that nested spelling to reuse the header.
template<>
void dsVECTOR<dsAFFIX_BUILDER::AFF_PARAM, 8>::ShrinkResize(int size)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            270,
            "size", size);

    if (size < this->nElem) {
        for (int index = size; index < this->nElem; ++index) {
            dsAFFIX_BUILDER::AFF_PARAM *param = &this->pData[index];
            dsTSTRING_BUF_HEADER<char> *valueBuffer = param->value.pBuffer;
            if (valueBuffer->refCount-- == 1)
                dlFree(valueBuffer);
            dsTSTRING_BUF_HEADER<char> *nameBuffer = param->name.pBuffer;
            if (--nameBuffer->refCount == 0)
                dlFree(nameBuffer);
        }
        this->nElem = size;
    }
}
