#include "../../headers/ws/ai/aiFILT_ENEMY.h"
#include "../../headers/ws/dbg/STRONG_ASSERT2_HELPER.h"

extern int IGNORE_STRONG_ASSERT; // ai08 strong-assert bypass. boundary.

// aiFILT_ENEMY::Pass(PASS_IFACE*) @ 0x832209F0
// ?Pass@aiFILT_ENEMY@@QAAXAAUPASS_IFACE@1@@Z
// Observe every element via Examine(), then keep only those Apply() accepts.
void aiFILT_ENEMY::Pass(PASS_IFACE *pass)
{
    int nElem = arr.nElem;
    for (int i = 0; i < nElem; ++i)
    {
        if (!IGNORE_STRONG_ASSERT && (i < 0 || i >= arr.nElem))
            STRONG_ASSERT2_HELPER::asserd<int, int>(
                "idx >= 0 && idx < nElem",
                "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp", 567,
                "idx", i, "nElem", arr.nElem);
        pass->Examine(&arr.pData[i]);
    }

    for (int i = arr.nElem - 1; i >= 0; --i)
    {
        if (!IGNORE_STRONG_ASSERT && i >= arr.nElem)
            STRONG_ASSERT2_HELPER::asserd<int, int>(
                "idx >= 0 && idx < nElem",
                "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp", 567,
                "idx", i, "nElem", arr.nElem);
        if (!pass->Apply(&arr.pData[i]))
            arr.Erase(i, 1);
    }
}
