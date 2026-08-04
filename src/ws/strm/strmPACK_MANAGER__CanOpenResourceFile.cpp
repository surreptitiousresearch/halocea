#include "../../headers/ws/strm/strmPACK_MANAGER.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

// strmPACK_MANAGER::CanOpenResourceFile @ 0x828F1EF0
// Linear scan over every registered pack for the first one whose CanOpenFile() accepts
// (resName, pakId); on a hit, write it out through *strmFile and return true.
bool strmPACK_MANAGER::CanOpenResourceFile(const dsSTRID &resName, int pakId, bool &cached,
                                            strmPACK_BASE **strmFile)
{
    if (packs.nElem <= 0)
        return false;

    int idx = 0;
    for (;;) {
        cached = false;
        if (!IGNORE_STRONG_ASSERT && (idx < 0 || idx >= packs.nElem))
            STRONG_ASSERT2_HELPER::asserd<int, int>("idx >= 0 && idx < nElem",
                "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp", 567,
                "idx", idx, "nElem", packs.nElem);

        if (packs.pData[idx]->CanOpenFile(resName, pakId, &cached))
            break;

        if (++idx >= packs.nElem)
            return false;
    }

    if (!IGNORE_STRONG_ASSERT && (idx < 0 || idx >= packs.nElem))
        STRONG_ASSERT2_HELPER::asserd<int, int>("idx >= 0 && idx < nElem",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp", 567,
            "idx", idx, "nElem", packs.nElem);

    *strmFile = packs.pData[idx];
    return true;
}
