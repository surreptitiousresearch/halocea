#include "dsVECTOR.h"
#include "dsSTRID.h"
#include "ds_assert_boundary.h"

// dsVECTOR<dsSTRID,8>::Resize(int,const dsSTRID&) @ 0x82CDFA40
// Resize the live count to `size`, copy-constructing (adopting `fill`'s interned id into) any
// newly exposed elements. Shrinking just drops the count (dsSTRID is trivially destructible).
template<>
void dsVECTOR<dsSTRID, 8>::Resize(int size, const dsSTRID &fill)
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
        this->nElem = size;
        return;
    }
    if (size == nElem)
        return;

    if (size <= this->allocated) {
        for (int index = nElem; index < size; ++index)
            this->pData[index].id = fill.id;
        this->nElem = size;
        return;
    }

    const char *fillId = fill.id; // save before Realloc may invalidate `fill` if it aliases storage
    this->Realloc(size);
    int reallocatedElem = this->nElem;
    for (int index = reallocatedElem; index < size; ++index)
        this->pData[index].id = fillId;
    this->nElem = size;
}
