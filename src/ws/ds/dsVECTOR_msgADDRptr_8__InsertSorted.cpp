#include "dsVECTOR.h"

class msgADDR; // boundary — msg-system endpoint address (element is a pointer)

// boundary — pointer comparator functor; ordering is by raw msgADDR* address, inlined below.
struct dsCMP;

// dsVECTOR<msgADDR*,8>::InsertSorted<dsCMP> @ 0x8255AB70
// Binary-search insert into an address-ascending pointer vector. On a miss, splice `val` at the
// lower-bound index and return it. On a match at `mid`, `dup` selects the duplicate policy
// (overwrite / ignore / insert-after); an unrecognized `dup` returns -1. The dsCMP comparator
// orders by pointer value (elem-vs-key), inlined as the three-way compare below.
template<>
template<>
int dsVECTOR<msgADDR *, 8>::InsertSorted<dsCMP>(
        msgADDR *const &val, const dsCMP & /*cmp*/, INS_SORT dup)
{
    int lo = 0;
    int hi = this->nElem;
    int order = 0;
    int mid = 0;
    while (true) {
        if (lo == hi) {
            this->Insert(lo, &val, 1);
            return lo;
        }
        mid = (lo + hi) >> 1;
        msgADDR *const *elem = &this->pData[mid];
        // dsCMP: +1 when elem sorts after val, 0 on equality, -1 when elem sorts before val.
        if (*elem >= val)
            order = (val < *elem);
        else
            order = -1;

        if (order > 0) {
            hi = mid;              // val sorts before mid
        } else if (order < 0) {
            lo = mid + 1;          // val sorts after mid
        } else {
            break;                 // exact match at mid
        }
    }

    int matchIndex = (lo + hi) >> 1; // == mid at the break
    if (dup == INS_DUP_REPLACE) {
        this->pData[mid] = val;
    } else if (dup == INS_DUP_IGNORE) {
        // keep the existing element
    } else if (dup == INS_DUP_RIGHT) {
        this->Insert(mid, &val, 1);
    } else {
        return -1;
    }
    return matchIndex;
}
