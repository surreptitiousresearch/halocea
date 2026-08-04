#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

class objOBJ; // boundary — obj subsystem game object (element is a pointer)

// dsVECTOR<objOBJ*,8>::Resize @ 0x82662648
// Resize the live count to `size`. When growing past capacity, Realloc grows the backing store;
// the newly exposed pointer elements are default-constructed, trivial (empty loop). When shrinking,
// elements above `size` are destroyed — a raw pointer is trivially destructible, so that loop is
// empty too. Either way the live count becomes `size`.
template<>
void dsVECTOR<objOBJ *, 8>::Resize(int size)
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
