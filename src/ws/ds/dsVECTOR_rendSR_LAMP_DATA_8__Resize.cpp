#include "dsVECTOR.h"
#include "../rend/rendSR_LAMP_DATA.h"
#include "ds_assert_boundary.h"

// dsVECTOR<rendSR_LAMP_DATA,8>::Resize(int) @ 0x829ECB68
// Resize the live count to `size`: shrinking just drops the count (trivially destructible);
// growing reallocates if needed then zero-initialises each newly exposed element's packed word.
template<>
void dsVECTOR<rendSR_LAMP_DATA, 8>::Resize(int size)
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
                this->pData[index].data = 0;
        }
    }
    this->nElem = size;
}
