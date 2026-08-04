#include "animINST.h"
#include "objOBJ_boundary.h"
#include "../ds/ds_assert_boundary.h"

// ?ApplyModifiers@animINST@@IAAXH@Z  (0x825E4720, protected) — re-apply object modifiers for every
// object recorded in objectsWithModifiersCache, propagating the validity mask `stateVld`.
void animINST::ApplyModifiers(int stateVld)
{
    dsVECTOR<short, 8> *pCache = &this->objectsWithModifiersCache;
    for (int i = 0; i < this->objectsWithModifiersCache.nElem; ++i)
    {
        int idx = i;
        if (!IGNORE_STRONG_ASSERT && (idx < 0 || idx >= pCache->nElem))
            STRONG_ASSERT2_HELPER::asserd<int, int>(
                "idx >= 0 && idx < nElem",
                "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
                567,
                "idx", idx,
                "nElem", pCache->nElem);

        objOBJ *pObj = this->ppObjList[pCache->pData[i]];
        if (pObj)
        {
            if (!IGNORE_STRONG_ASSERT && !pObj->pGeom)
                STRONG_ASSERT_DUMMY().Crash(
                    "po->IsGeom()",
                    "D:\\Projects\\code\\common\\src.sys\\animation\\anim.cpp",
                    2146,
                    dsStrongAssertMessage);
            pObj->ApplyModifiers(stateVld);
        }
    }
}
