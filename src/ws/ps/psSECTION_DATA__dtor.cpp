#include "psSECTION_DATA.h"
#include "../ds/dsDATA.h"
#include "../ds/dsVECTOR.h"
// @0x82519828 — release/free every owned sub-section (recursively deleting bodies this section owns
// directly), then tear down the record and parent-list vectors. `__nPsData` is the global live-count
// of allocated psSECTION_DATA bodies, decremented here.

extern int __nPsData; // boundary — global live psSECTION_DATA count

psSECTION_DATA::~psSECTION_DATA()
{
    for (int i = 0; i < orderedRecords.nElem; ++i) {
        psSECTION_DATA *subSection = GetSubSection(i);
        if (subSection) {
            dsDATA empty;
            empty.type = nullptr;
            orderedRecords[i].value.StoreValue(empty);
            subSection->~psSECTION_DATA();
            operator delete(subSection);
        }
    }

    --__nPsData;
    orderedRecords.~dsVECTOR<psSECTION_RECORD, 8>();
    psParents.~dsVECTOR<psSECTION, 8>();
}
