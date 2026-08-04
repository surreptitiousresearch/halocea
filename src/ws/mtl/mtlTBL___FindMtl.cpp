#include "mtlTBL.h"
#include "../ds/ds_assert_boundary.h"

// mtlTBL::_FindMtl @ 0x82C2B3A8 (const)
// Linear scan of mtlList for the slot whose `mtl` key matches. direction selects the scan
// order: 0 = forward from startPos (bounds-checked via the inlined ds_vector.hpp:557
// STRONG_ASSERT2_HELPER::asserd "idx >= 0 && idx < nElem" accessor on each iteration),
// 1 = backward from (nElem - startPos - 1) down to 0. Any other direction value falls
// through to the default "not found" return. Returns the matching slot index, or -1.
int mtlTBL::_FindMtl(int mtl, int startPos, int direction) const
{
    if (direction == 0)
    {
        int nElem = this->mtlList.nElem;
        if (startPos >= nElem)
            return -1;

        int scan_index = startPos;
        for (;;)
        {
            // Bounds-checked element access (inlined dsVECTOR<mtlMTL,8>::operator[]).
            if (!IGNORE_STRONG_ASSERT && (scan_index < 0 || scan_index >= nElem))
                STRONG_ASSERT2_HELPER::asserd("idx >= 0 && idx < nElem",
                                               "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
                                               557, "idx", scan_index, "nElem", nElem);
            if (this->mtlList.pData[scan_index].mtl == mtl)
                return scan_index;

            nElem = this->mtlList.nElem; // re-read: nElem may change if a strong-assert path re-enters
            if (++scan_index >= this->mtlList.nElem)
                return -1;
        }
    }
    else if (direction == 1)
    {
        int scan_index = this->mtlList.nElem - startPos - 1;
        if (scan_index < 0)
            return -1;

        while (this->mtlList[scan_index].mtl != mtl)
        {
            if (--scan_index < 0)
                return -1;
        }
        return scan_index;
    }

    return -1;
}
