#include "dsVECTOR.h"
#include "ds_assert_boundary.h"
#include "dsDATA.h"

// dsVECTOR<dsDATA,8>::Resize @ 0x825A99A0
// Resize the live count to `size`. When growing, newly exposed dsDATA slots are default-constructed
// (type pointer nulled = empty value). When shrinking, each element above `size` is destroyed by
// dispatching its stored type's Destroy hook (empty values have a null type and are skipped).
template<>
void dsVECTOR<dsDATA, 8>::Resize(int size)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            211,
            "size",
            size);
    if (size >= this->nElem) {
        if (size > this->nElem) {
            if (size > this->allocated)
                this->Realloc(size);
            for (int growIndex = this->nElem; growIndex < size; ++growIndex)
                if (&this->pData[growIndex])
                    this->pData[growIndex].type = nullptr;
        }
        this->nElem = size;
    } else {
        for (int shrinkIndex = size; shrinkIndex < this->nElem; ++shrinkIndex) {
            const dsDATA_TYPE *storedType = this->pData[shrinkIndex].type;
            if (storedType)
                storedType->Destroy(&this->pData[shrinkIndex].storage);
        }
        this->nElem = size;
    }
}
