#include "dsVECTOR.h"
#include "ds_assert_boundary.h"
#include "dsStrIntMapERASED.h"

// dsVECTOR<dsERASE_MANAGER<...int-map LINE_ERASER>::ERASED,8>::Resize @ 0x826183E8
// Resize the live count to `size`. When growing past capacity, Realloc grows the backing store;
// the newly exposed ERASED records are default-constructed, trivial for a POD (empty loop). When
// shrinking, elements above `size` are destroyed — ERASED has a trivial destructor, so that loop
// is empty too. Either way the live count becomes `size`.
template<>
void dsVECTOR<dsStrIntMapERASED, 8>::Resize(int size)
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
        // grow: default-construct new elements [nElem, size) — trivial POD (empty loop)
    } else {
        // shrink: destroy elements [size, nElem) — trivial ERASED destructor (empty loop)
        for (int i = size; i < this->nElem; ++i)
            ;
    }
    this->nElem = size;
}
