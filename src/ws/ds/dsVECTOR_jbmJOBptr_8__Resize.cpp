#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

struct jbmJOB; // boundary — job-manager work item (jbm subsystem); only a pointer is handled

// dsVECTOR<jbmJOB*,8>::Resize @ 0x826EC360
// Resize the live count to `size`. When growing past capacity, Realloc grows the backing store; the
// newly exposed pointer slots are default-constructed, trivial (empty loop). When shrinking, slots
// above `size` are destroyed — a raw pointer has a trivial destructor, so that loop is empty too.
// Either way the live count becomes `size`.
template<>
void dsVECTOR<jbmJOB *, 8>::Resize(int size)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            211,
            "size", size);
    if (size >= this->nElem) {
        if (size > this->nElem && size > this->allocated)
            this->Realloc(size);
        // grow: default-construct new pointer slots [nElem, size) — trivial (empty loop)
    } else {
        // shrink: destroy pointer slots [size, nElem) — trivial destructor (empty loop)
        for (int i = size; i < this->nElem; ++i)
            ;
    }
    this->nElem = size;
}
