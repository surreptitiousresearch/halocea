#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

// dsVECTOR<bool,8>::Resize(int,const bool&) @ 0x82522700  (fill overload)
// Lowest-address of the two dsVECTOR<bool,8>::Resize overloads, so it takes the bare filename; the
// plain Resize(int) lives at 0x82D9F918.
// Resize the live count to `size`, copy-filling any newly exposed slots with `fill`. When shrinking,
// the elements above `size` would be destroyed — bool is trivially destructible, so that loop is
// empty and only the count drops.
template<>
void dsVECTOR<bool, 8>::Resize(int size, const bool &fill)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            238,
            "size",
            size);

    int nElem = this->nElem;
    if (size < nElem) {
        // shrink: destroy elements [size, nElem) — trivial for bool (empty loop)
        for (int shrinkIndex = size; shrinkIndex < nElem; ++shrinkIndex)
            ;
        this->nElem = size;
        return;
    }
    if (size <= nElem) {
        this->nElem = size;
        return;
    }
    if (size <= this->allocated) {
        // spare capacity: fill in place
        for (int fillIndex = nElem; fillIndex < size; ++fillIndex)
            if (&this->pData[fillIndex])
                this->pData[fillIndex] = fill;
        this->nElem = size;
        return;
    }
    // grow the backing store first (save `fill`, it may alias the storage being reallocated)
    bool savedFill = fill;
    this->Realloc(size);
    for (int fillIndex = this->nElem; fillIndex < size; ++fillIndex)
        if (&this->pData[fillIndex])
            this->pData[fillIndex] = savedFill;
    this->nElem = size;
}
