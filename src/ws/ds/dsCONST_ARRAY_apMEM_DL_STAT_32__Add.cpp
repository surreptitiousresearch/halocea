#include "dsCONST_ARRAY.h"
#include "../ap/apMEM_DL_STAT.h"
#include "ds_assert_boundary.h"

// dsCONST_ARRAY<apMEM_DL_STAT,32>::Add @ 0x82C4B920
// Append a copy of `val` to the inline array, asserting capacity first. The original writes
// the two int fields directly into the next entry slot via base+2*nEntry pointer math; this
// is the typed equivalent (entry stride is 2 ints = sizeof(apMEM_DL_STAT)).
template<>
void dsCONST_ARRAY<apMEM_DL_STAT, 32>::Add(const apMEM_DL_STAT &val)
{
    if (!IGNORE_STRONG_ASSERT && this->nEntry >= 32)
        reinterpret_cast<STRONG_ASSERT_DUMMY *>(0)->Crash(
            "Length() < size",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_const_list.h",
            175,
            dsStrongAssertMessage);
    apMEM_DL_STAT &slot = this->list[this->nEntry];
    ++this->nEntry;
    slot.type = val.type;
    slot.size = val.size;
}
