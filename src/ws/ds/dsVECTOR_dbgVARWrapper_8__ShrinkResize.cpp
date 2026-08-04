#include "dsVECTOR.h"
#include "../wb/dbgVARWrapper.h"
#include "ds_assert_boundary.h"

// boundary — dbgVAR_SIMPLE<bool,1>: a polymorphic debug-variable node owned by the wrapper.
// Modeled only through its virtual destructor (the wrapper stores it as void*).
struct dbgVARnode { virtual ~dbgVARnode(); };

// dsVECTOR<wb::dbgVARWrapper,8>::ShrinkResize @ 0x8269DCF0
// Shrink the live count to `size`, destroying each wrapper above the new count. The wrapper owns
// its dbgVAR node: invoke the node's scalar-deleting destructor (flag 1 = free) and null the slot.
template<>
void dsVECTOR<wb::dbgVARWrapper, 8>::ShrinkResize(int size)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            270,
            "size", size);

    if (size < this->nElem) {
        for (int index = size; index < this->nElem; ++index) {
            wb::dbgVARWrapper *wrapper = &this->pData[index];
            if (wrapper->dbgVar) {
                delete static_cast<dbgVAR *>(wrapper->dbgVar); // polymorphic base (was misnamed dbgVARnode)
                wrapper->dbgVar = nullptr;
            }
        }
        this->nElem = size;
    }
}
