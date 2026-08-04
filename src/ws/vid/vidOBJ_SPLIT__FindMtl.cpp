#include "vidOBJ_SPLIT.h"
#include "../ds/ds_assert_boundary.h"

// 0x82C87370 (const)
// Linear scan of mtlList for the slot whose fixed-size `name` buffer matches the NUL-terminated
// text of `name` (a manual byte-by-byte strcmp against vidMTL::name, terminating on the
// candidate's own NUL -- equivalent to strcmp(candidateName, name.CStr()) == 0).
int vidOBJ_SPLIT::FindMtl(const dsTSTRING<char> &name) const
{
    if (mtlList.nElem == 0)
        return -1;

    const char *targetStr = name.pBuffer->str;

    for (int candidateIndex = 0; ; ++candidateIndex)
    {
        if (!IGNORE_STRONG_ASSERT && (candidateIndex < 0 || candidateIndex >= mtlList.nElem))
            STRONG_ASSERT2_HELPER::asserd(
                "idx >= 0 && idx < nElem",
                "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
                557, "idx", candidateIndex, "nElem", mtlList.nElem);

        const char *candidateName = mtlList.pData[candidateIndex].name;
        const char *targetChar    = targetStr;
        int diff;

        for (;;)
        {
            unsigned char candidateChar = (unsigned char)*candidateName;
            diff = candidateChar - (unsigned char)*targetChar;
            if (candidateChar == 0)
                break;
            ++candidateName;
            ++targetChar;
            if (diff != 0)
                break;
        }

        if (diff == 0)
            return candidateIndex;

        if (candidateIndex + 1 == mtlList.nElem)
            return -1;
    }
}
