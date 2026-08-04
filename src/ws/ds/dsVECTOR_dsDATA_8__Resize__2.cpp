#include "dsVECTOR.h"
#include "dsDATA.h"
#include "ds_assert_boundary.h"

// Global placement copy-construct helper, already reversed for dsDATA in New_dsDATA.cpp:
// null-inits *place's type then StoreValue-copies *val into it. Forward-declared (primary
// template) here to type-check the calls below, matching the disasm's calls into that same
// specialization.
template<class T> T *New(T *place, const T *val);

// dsVECTOR<dsDATA,8>::Resize (fill overload: size, const dsDATA& fill) @ 0x8308ADF0
// Resize the live count to `size`. When shrinking, each element above `size` is destroyed via its
// stored type's Destroy hook (empty values are skipped). When growing: if capacity already covers
// `size`, each newly exposed slot is copy-constructed directly from `fill` via New<dsDATA>.
// Otherwise `fill` is captured into a stack scratch FIRST (it may alias storage about to be
// reallocated), the backing store is grown, each newly exposed slot is copy-constructed from the
// scratch, and the scratch is released.
template<>
void dsVECTOR<dsDATA, 8>::Resize(int size, const dsDATA &fill)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            238,
            "size", size);

    int nElem = this->nElem;
    if (size >= nElem) {
        if (size > nElem) {
            if (size <= this->allocated) {
                for (int i = nElem; i < size; ++i)
                    New<dsDATA>(&this->pData[i], &fill);
            } else {
                dsDATA staged;
                staged.type = nullptr;
                staged.StoreValue(fill);

                if (size > this->allocated)
                    this->Realloc(size);

                int grown = this->nElem;
                for (int i = grown; i < size; ++i)
                    New<dsDATA>(&this->pData[i], &staged);

                if (staged.type)
                    staged.type->Destroy((unsigned int *)&staged);
            }
        }
    } else {
        for (int i = size; i < this->nElem; ++i) {
            const dsDATA_TYPE *type = this->pData[i].type;
            if (type)
                type->Destroy(&this->pData[i].storage);
        }
    }
    this->nElem = size;
}
