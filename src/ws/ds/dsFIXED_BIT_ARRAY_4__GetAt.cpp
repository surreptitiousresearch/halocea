#include "dsFIXED_BIT_ARRAY.h"
#include "ds_assert_boundary.h"

// dsFIXED_BIT_ARRAY<4>::GetAt @ 0x826C1560
// Bounds-checked read of cell `index` (asserts index < NELEM, then reads regardless).
template<>
unsigned int dsFIXED_BIT_ARRAY<4>::GetAt(unsigned int index) const
{
    if (!IGNORE_STRONG_ASSERT && index >= 4)
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
            "i >= 0 && i < NELEM",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_bit_array.h",
            132,
            empty_string);
    return this->array[index];
}
