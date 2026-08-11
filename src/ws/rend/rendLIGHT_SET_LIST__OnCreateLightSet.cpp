#include "../../headers/ws/rend/rendLIGHT_SET_LIST.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

// 0x82ABDBB0 — register a newly created light set. Under the list lock, walks the existing
// entries asserting pLS is not already present (a duplicate registration is a contract
// violation), then appends pLS to the list.
// NOTE: the decompiler emitted 11 extra phantom int parameters (a3..a13) beyond (this, pLS);
// disasm confirms the real ABI is exactly (this in r3, pLS in r4) matching the QAAXPAV1@@Z
// mangle, so those are decompiler-register-pollution artifacts and are dropped here.
void rendLIGHT_SET_LIST::OnCreateLightSet(rendLIGHT_SET *pLS)
{
    lock.Lock(nullptr, 0);

    for (int lightSetIndex = 0; lightSetIndex < lightSets.nElem; ++lightSetIndex)
    {
        if (!IGNORE_STRONG_ASSERT && lightSets[lightSetIndex] == pLS)
        {
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
                "lightSets[i] != pLS",
                "D:\\Projects\\code\\common\\src.sys\\render\\Rend_light_set.cpp",
                208,
                empty_string);
        }
    }

    lightSets.PushBack(pLS);

    lock.Unlock(nullptr, 0);
}
