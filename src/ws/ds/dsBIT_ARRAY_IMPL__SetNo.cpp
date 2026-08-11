#include "dsBIT_ARRAY_IMPL.h"
#include "ds_assert_boundary.h"

// dsBIT_ARRAY_IMPL::SetNo @ 0x8253C060
// Clear bit `i` in the word array; word index = i>>5 (Bit2Idx), bit mask = 1<<(i&31).
void dsBIT_ARRAY_IMPL::SetNo(int i, unsigned int *array, int n_elem)
{
    if (!IGNORE_STRONG_ASSERT && (i < 0 || (i >> 5) >= n_elem))
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
            "i >= 0 && Bit2Idx(i) < n_elem",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_bit_array.h",
            31,
            empty_string);
    array[i >> 5] &= ~(1 << (i & 0x1F));
}
