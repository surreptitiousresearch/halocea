#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

// dsVECTOR<char,8>::Resize @ 0x823B7CC8
// Resize the live count to `size`. When growing past capacity, Realloc grows the backing store,
// then the newly exposed elements would be default-constructed — char is trivially constructible,
// so that loop is empty. When shrinking, the elements above `size` would be destroyed — char is
// trivially destructible, so that loop is empty too. Either way the live count becomes `size`.
template<>
void dsVECTOR<char, 8>::Resize(int size)
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
        // grow: default-construct new elements [nElem, size) — trivial for char (empty loop)
    } else {
        // shrink: destroy elements [size, nElem) — trivial for char (empty loop)
        for (int i = size; i < this->nElem; ++i)
            ;
    }
    this->nElem = size;
}
