#include "dsCONST_ARRAY.h"
#include "../ap/apMEM_DL_STAT.h"

// dsCONST_ARRAY<apMEM_DL_STAT,32>::Back @ 0x82C4B8A0
// Reference to the last live entry (index nEntry-1); assert 0 < nEntry <= 32 first.
template<>
apMEM_DL_STAT &dsCONST_ARRAY<apMEM_DL_STAT, 32>::Back()
{
    if (!IGNORE_STRONG_ASSERT && (this->nEntry <= 0 || this->nEntry > 32))
        STRONG_ASSERT_DUMMY().Crash(
            "nEntry > 0 && nEntry <= size",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_const_list.h",
            39,
            dsStrongAssertMessage);
    return this->list[this->nEntry - 1];
}
