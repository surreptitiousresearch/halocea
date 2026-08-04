#include "dsVECTOR.h"
#include "../prop/instCONSTRUCTOR_NODE_RELATIONS.h"
#include "ds_assert_boundary.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<instCONSTRUCTOR_NODE_RELATIONS::RELATION,8>::ShrinkResize @ 0x826DF488
// Shrink the live count to `size`, destroying every dropped element's child then parent string
// (drop each dsTSTRING buffer reference, freeing at zero). translation/rotation/scale (m3dV) are
// plain floats — nothing to destroy.
template<>
void dsVECTOR<instCONSTRUCTOR_NODE_RELATIONS::RELATION, 8>::ShrinkResize(int size)
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
            instCONSTRUCTOR_NODE_RELATIONS::RELATION *elem = &this->pData[i];
            dsTSTRING_BUF_HEADER<char> *childBuffer = elem->child.pBuffer;
            if (childBuffer->refCount-- == 1)
                dlFree(childBuffer);
            dsTSTRING_BUF_HEADER<char> *parentBuffer = elem->parent.pBuffer;
            if (--parentBuffer->refCount == 0)
                dlFree(parentBuffer);
        }
        this->nElem = size;
    }
}
