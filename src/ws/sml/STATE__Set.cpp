#include "STATE.h"
#include "../ds/dsBIT_ARRAY_IMPL.h"
#include "../ds/ds_assert_boundary.h"

namespace sml {

// ?Set@STATE@sml@@QAAXH@Z (@0x826661F0) — set the active state by numeric id: a direct
// bit-set into the 64-bit (dsFIXED_BIT_ARRAY<2>) active-state mask. Bounds-checked; an
// out-of-range id (>= 64) hits the strong-assert crash path instead of being set.
void STATE::Set(int idxState)
{
    if (idxState >= 0)
    {
        if (idxState < 64)
        {
            // state.array is the raw word array backing the bit set; &this->state == &state.array
            // since `state` is the first member, so passing `state.array` is exactly
            // this->state.array (dsFIXED_BIT_ARRAY<2> == 2 * 32 = 64 bits).
            dsBIT_ARRAY_IMPL::SetYes(idxState, state.array, 2);
        }
        else if (!IGNORE_STRONG_ASSERT)
        {
            // D:\Projects\code\common\src.sys\gm_sys\entity\ia_sml_state.cpp line 250 —
            // unconditional STRONG_ASSERT(false) on the out-of-range branch.
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
                "false",
                "D:\\Projects\\code\\common\\src.sys\\gm_sys\\entity\\ia_sml_state.cpp",
                250,
                empty_string);
        }
    }
}

} // namespace sml
