#include "dsVECTOR.h"
#include "PTR_LIST.h"
#include "ds_assert_boundary.h"

struct msgADDR; // boundary — msg-system endpoint address (msg subsystem)

// dsVECTOR<ds::PTR_LIST<msgADDR>,8>::ShrinkResize @ 0x825591F0
// Shrink the live count to `size`. Each element is a PTR_LIST head record (pHead/pTail/length)
// holding no owned storage, so the per-element destructor loop is empty and collapses to
// clamping the count.
template<>
void dsVECTOR<ds::PTR_LIST<msgADDR>, 8>::ShrinkResize(int size)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            270,
            "size", size);
    if (size < this->nElem)
        this->nElem = size;
}
