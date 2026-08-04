#include "../../headers/ws/ds/dsCONST_ARRAY.h"
#include "../../headers/hcex/hcexJOB_SYNC_INST.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

// dsCONST_ARRAY<hcexJOB_SYNC_INST,4>::operator[] (non-const) @ 0x823D1CE8
// ??A?$dsCONST_ARRAY@VhcexJOB_SYNC_INST@@$03@@QAAAAVhcexJOB_SYNC_INST@@H@Z
// STRONG_ASSERT-guarded mutable access; asserts IsValidIdx(idx) before returning the slot ref.
template<>
hcexJOB_SYNC_INST &dsCONST_ARRAY<hcexJOB_SYNC_INST, 4>::operator[](int idx)
{
    if (!IGNORE_STRONG_ASSERT && ((unsigned int)idx >= 4 || idx >= this->nEntry))
        STRONG_ASSERT_DUMMY::Crash(
            nullptr,
            "IsValidIdx(idx)",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_const_list.h",
            166,
            (unsigned char)dsStrongAssertMessage[0]);
    return this->list[idx];
}
