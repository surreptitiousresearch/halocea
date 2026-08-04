#include "dsVECTOR.h"
#include "../ssl/sslCLASS_ELEMENT.h"
#include "ds_assert_boundary.h"

// dsVECTOR<sslCLASS_ELEMENT,8>::ShrinkResize @ 0x82A38738
// Shrink the live count to `size`. sslCLASS_ELEMENT is trivially destructible (a plain POD
// record), so the generic per-element destructor loop is empty and collapses to the count.
template<>
void dsVECTOR<sslCLASS_ELEMENT, 8>::ShrinkResize(int size)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            270,
            "size",
            size);
    if (size < this->nElem)
        this->nElem = size;
}
