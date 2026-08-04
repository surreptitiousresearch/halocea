#include "psSECTION_INTERFACE.h"
#include "psSECTION_DATA.h"
// 0x8251F0F8 — shrink both the parent-list and record tables to exactly fit their live counts.

void psSECTION_INTERFACE::Compact()
{
    psSECTION_DATA *data = this->pData;
    if (data) {
        data->psParents.Realloc(data->psParents.nElem);
        data->orderedRecords.Realloc(data->orderedRecords.nElem);
    }
}
