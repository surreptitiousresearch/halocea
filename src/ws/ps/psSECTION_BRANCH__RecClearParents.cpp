#include "psSECTION_BRANCH.h"
#include "psSECTION_DATA.h"
// 0x82519180 — clear `pData`'s parent list and recurse into every sub-section, tearing down the
// parent back-references across the whole branch. Protected (IAA).

void psSECTION_BRANCH::RecClearParents(psSECTION_DATA *pData)
{
    pData->psParents.Clear();
    for (int recordIndex = 0; recordIndex < pData->orderedRecords.nElem; ++recordIndex)
    {
        psSECTION_DATA *subSection = pData->GetSubSection(recordIndex);
        if (subSection)
            this->RecClearParents(subSection);
    }
}
