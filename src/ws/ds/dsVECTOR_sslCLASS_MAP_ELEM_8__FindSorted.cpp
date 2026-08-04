#include "dsVECTOR.h"
#include "dsSTRID.h"

// boundary — ssl class-name map entry; DB layout: name@0 (dsSTRID), cls@4 (sslCLASS_REF).
struct sslCLASS_MAP_ELEM { dsSTRID name; void *cls; };
struct sslCMP_NAMES {}; // boundary — comparator over interned name ids (compare inlined)

// dsVECTOR<sslCLASS_MAP_ELEM,8>::FindSorted<sslCMP_NAMES,dsSTRID> @ 0x825CA828
// Binary search by interned-name pointer identity for `key`; returns its index or -1.
template<>
template<>
int dsVECTOR<sslCLASS_MAP_ELEM, 8>::FindSorted<sslCMP_NAMES, dsSTRID>(
        const dsSTRID &key, const sslCMP_NAMES &cmp) const
{
    (void)cmp;
    int hi = this->nElem;
    int lo = 0;
    if (hi) {
        const char *targetId = key.id;
        const sslCLASS_MAP_ELEM *elems = this->pData;
        do {
            int mid = (hi + lo) >> 1;
            const char *midId = elems[mid].name.id;
            if (midId < targetId)
                lo = mid + 1;
            else if (targetId >= midId) // equal (interned-pointer identity)
                return mid;
            else
                hi = mid;
        } while (lo != hi);
    }
    return -1;
}
