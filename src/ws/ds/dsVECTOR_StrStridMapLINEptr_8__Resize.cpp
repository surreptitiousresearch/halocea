#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

// boundary — dsLF_SORTED_MAP<const char*,dsSTRID,dsSTR_HASH,dsSTR_CMP>::LINE hash-chain node.
// Opaque here; the vector element is a pointer to it.
struct dsStrStridMapLINE;

// dsVECTOR<dsStrStridMapLINE*,8>::Resize @ 0x8252AB10
// Resize the live count to `size`. When growing past capacity, Realloc grows the backing store;
// the newly exposed pointer elements are default-constructed, which is trivial (empty loop).
// When shrinking, elements above `size` are destroyed — a raw pointer is trivially destructible,
// so that loop is empty too. Either way the live count becomes `size`.
template<>
void dsVECTOR<dsStrStridMapLINE *, 8>::Resize(int size)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            211,
            "size",
            size);
    if (size >= this->nElem) {
        if (size > this->nElem && size > this->allocated)
            this->Realloc(size);
        // grow: default-construct new elements [nElem, size) — trivial for a pointer (empty loop)
    } else {
        // shrink: destroy elements [size, nElem) — trivial for a pointer (empty loop)
        for (int i = size; i < this->nElem; ++i)
            ;
    }
    this->nElem = size;
}
