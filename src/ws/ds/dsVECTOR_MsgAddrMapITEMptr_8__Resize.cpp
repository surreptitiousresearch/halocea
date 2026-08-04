#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

#include "dsMsgAddrMapITEM.h"

// dsVECTOR<...msgADDR-map ITEM*,8>::Resize @ 0x8255AD80
// Fill overload: resize the live count to `size`. Growing exposes new slots, each set to the single
// pointer `fill` (reallocating first when `size` exceeds capacity); shrinking just drops the trailing
// slots. The element is a raw pointer (trivially destructible), so the shrink loop is empty.
template<>
void dsVECTOR<dsMsgAddrMapITEM *, 8>::Resize(int size, dsMsgAddrMapITEM *const &fill)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            238,
            "size", size);

    if (size >= this->nElem) {
        if (size > this->nElem) {
            if (size <= this->allocated) {
                for (int slotIdx = this->nElem; slotIdx < size; ++slotIdx) {
                    dsMsgAddrMapITEM **slot = &this->pData[slotIdx];
                    if (slot)
                        *slot = fill;
                }
            } else {
                dsMsgAddrMapITEM *fillCopy = fill; // cache before Realloc frees the old storage
                this->Realloc(size);
                for (int slotIdx = this->nElem; slotIdx < size; ++slotIdx) {
                    dsMsgAddrMapITEM **slot = &this->pData[slotIdx];
                    if (slot)
                        *slot = fillCopy;
                }
            }
        }
    } else {
        for (int slotIdx = size; slotIdx < this->nElem; ++slotIdx) // trivial pointer destructor (empty)
            ;
    }
    this->nElem = size;
}
