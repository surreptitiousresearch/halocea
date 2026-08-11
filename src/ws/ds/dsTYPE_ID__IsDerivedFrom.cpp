#include "dsTYPE_ID.h"
#include "ds_assert_boundary.h"

// dsTYPE_ID::IsDerivedFrom @ 0x826C1710
// Return 1 iff this type-id derives from `baseTypeID`: for every 32-bit cell, base's set bits
// must be a subset of this->id's set bits (base.id[i] & this.id[i] == base.id[i]).
// const method (QBA mangle). Asserts the base id is initialised (non-zero) first.
int dsTYPE_ID::IsDerivedFrom(const dsTYPE_ID &baseTypeID) const
{
    if (!IGNORE_STRONG_ASSERT) {
        dsFIXED_BIT_ARRAY<4> zero; // all-zero comparand
        zero.array[0] = 0;
        zero.array[1] = 0;
        zero.array[2] = 0;
        zero.array[3] = 0;
        if (!(baseTypeID.id != zero)) // base id equals zero => not initialised
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
                "baseTypeID.IsInited()",
                "D:\\Projects\\code\\common\\src.sys\\ds\\ds_typeid.cpp",
                52,
                empty_string);
    }

    for (int cell = 0; cell < 4; ++cell) {
        unsigned int baseBits = baseTypeID.id.GetAt(cell);
        unsigned int commonBits = baseBits & this->id.GetAt(cell);
        if (commonBits != baseTypeID.id.GetAt(cell))
            return 0; // base has a bit this type lacks: not derived
    }
    return 1;
}
