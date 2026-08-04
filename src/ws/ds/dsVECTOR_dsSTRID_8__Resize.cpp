#include "dsVECTOR.h"
#include "dsSTRID.h"
#include "ds_assert_boundary.h"
#include <new> // placement new

// dsVECTOR<dsSTRID,8>::Resize(int) @ 0x82CDFB60
// Resize the live count to `size`: shrinking drops the count (dsSTRID is trivially destructible);
// growing reallocates if needed then default-constructs each newly exposed element (adopts the
// interned empty-string id).
template<>
void dsVECTOR<dsSTRID, 8>::Resize(int size)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            211,
            "size",
            size);

    int nElem = this->nElem;
    if (size >= nElem) {
        if (size > nElem) {
            if (size > this->allocated)
                this->Realloc(size);
            for (int index = this->nElem; index < size; ++index)
                new (&this->pData[index]) dsSTRID();
        }
        this->nElem = size;
    } else {
        this->nElem = size;
    }
}
