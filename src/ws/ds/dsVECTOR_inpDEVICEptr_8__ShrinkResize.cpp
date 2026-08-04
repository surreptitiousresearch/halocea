#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

struct inpDEVICE; // boundary — ws-engine inp: physical input device (element is a pointer)

// dsVECTOR<inpDEVICE*,8>::ShrinkResize @ 0x826402D8
// Shrink the live count to `size`. The element is a raw pointer (trivially destructible), so
// the generic per-element destructor loop body is empty; only the count is reset.
template<>
void dsVECTOR<inpDEVICE *, 8>::ShrinkResize(int size)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            270,
            "size",
            size);
    if (size < this->nElem) {
        for (int i = size; i < this->nElem; ++i)
            ; // trivially destructible pointer element; destructor loop emitted as a no-op
        this->nElem = size;
    }
}
