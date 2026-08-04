#include "psSECTION_INTERFACE.h"
#include "psSECTION_DATA.h"
#include "psSECTION_RECORD.h"
// 0x82519528 — source line number of key/record `idx`: 0 when the handle is null, -1 when out of
// range, otherwise the record's parsed line number. const (QBA).

int psSECTION_INTERFACE::GetKeyLineNmb(int idx) const
{
    psSECTION_DATA *pData = this->pData;
    if (!pData)
        return 0;
    if (idx < 0 || idx >= pData->orderedRecords.nElem)
        return -1;
    return pData->orderedRecords[idx].lineNmb;
}
