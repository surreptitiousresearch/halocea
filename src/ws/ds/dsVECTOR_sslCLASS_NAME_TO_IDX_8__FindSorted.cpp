#include "dsVECTOR.h"
#include "dsSTRID.h"

// boundary — sslCLASS::NAME_TO_IDX; DB layout: name@0 (dsSTRID), idx@4 (int).
namespace sslCLASS_ns { struct NAME_TO_IDX { dsSTRID name; int idx; }; }
using sslCLASS_NAME_TO_IDX = sslCLASS_ns::NAME_TO_IDX; // spelled sslCLASS::NAME_TO_IDX in the binary
struct dsCMP {}; // boundary — comparator over interned name ids (compare inlined)

// dsVECTOR<sslCLASS::NAME_TO_IDX,8>::FindSorted<dsCMP,dsSTRID> @ 0x82A37F58
// Binary search by interned-name pointer identity for `key`; returns its index or -1.
template<>
template<>
int dsVECTOR<sslCLASS_NAME_TO_IDX, 8>::FindSorted<dsCMP, dsSTRID>(
        const dsSTRID &key, const dsCMP &cmp) const
{
    (void)cmp;
    int hi = this->nElem;
    int lo = 0;
    if (hi) {
        const sslCLASS_NAME_TO_IDX *elems = this->pData;
        const char *targetId = key.id;
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
