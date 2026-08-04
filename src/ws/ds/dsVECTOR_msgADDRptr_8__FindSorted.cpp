#include "dsVECTOR.h"

struct msgADDR; // boundary — msg-system endpoint address (msg subsystem); only pointers are ordered

// boundary — pointer comparator functor; ordering is by raw msgADDR* address, inlined below.
struct dsCMP;

// dsVECTOR<msgADDR*,8>::FindSorted<dsCMP,msgADDR*> @ 0x82557A48
// Binary search of the address-ascending pointer vector for the element equal to *key; returns its
// index or -1. The dsCMP comparator orders by pointer value (elem-vs-key), inlined here. const (QBA).
template<>
template<>
int dsVECTOR<msgADDR *, 8>::FindSorted<dsCMP, msgADDR *>(
        msgADDR *const &key, const dsCMP & /*cmp*/) const
{
    int lo = 0;
    int hi = this->nElem;
    while (lo != hi) {
        int mid = (lo + hi) >> 1;
        msgADDR *const *elem = &this->pData[mid];
        int order;
        if (*elem >= key)          // pointer-value ordering (key is msgADDR* const&)
            order = (key < *elem); // >0 when elem sorts after key, 0 on equality
        else
            order = -1;             // elem sorts before key

        if (order <= 0) {
            if (order >= 0)
                return mid;   // exact match
            lo = mid + 1;     // key sorts after mid
        } else {
            hi = mid;         // key sorts before mid
        }
    }
    return -1;
}
