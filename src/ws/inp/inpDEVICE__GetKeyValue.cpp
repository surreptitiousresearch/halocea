// inpDEVICE::GetKeyValue @0x8263EE30 — read the analog value of an input key/axis.
// Returns 0 when the key is masked out (keyState bit 3 = suppressed/consumed), else the raw
// stored analog value. Strong-asserts the key index is in [0, 340).
#include "../../headers/ws/inp/inpDEVICE.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

float inpDEVICE::GetKeyValue(int key) const
{
    if (!IGNORE_STRONG_ASSERT)
    {
        bool isValidKey = (key >= 340 || key < 0) ? false : true;
        if (!isValidKey)
            // Null-this call matches the decompiler's leading-nullptr this arg; Crash is a member.
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
                "IsValidKey(key)",
                "D:\\Projects\\code\\common\\src.sys\\drv\\input\\inp.cpp",
                177, empty_string);
    }

    float value;
    if ((keyState[key] & 8) != 0)
        value = 0.0f;
    else
        value = keyValue[key];
    return value;
}
