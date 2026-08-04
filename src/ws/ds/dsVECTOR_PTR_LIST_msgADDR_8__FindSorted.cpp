#include "dsVECTOR.h"
#include "PTR_LIST.h"

struct msgADDR; // boundary — msg subsystem message address

// boundary — msg comparator: orders a message-address bucket (PTR_LIST) against a target address.
// Returns <0 / 0 / >0 like strcmp (element-vs-key).
struct msgCMP_ADDR1 {
    int cmp(const ds::PTR_LIST<msgADDR> *elem, msgADDR *key) const; // boundary
};

// dsVECTOR<ds::PTR_LIST<msgADDR>,8>::FindSorted<msgCMP_ADDR1,msgADDR*> @ 0x82558990
// Binary search of the ascending-sorted vector for the bucket matching `key`; returns its index
// or -1. const method (QBA mangle).
template<>
template<>
int dsVECTOR<ds::PTR_LIST<msgADDR>, 8>::FindSorted<msgCMP_ADDR1, msgADDR *>(
        msgADDR *const &key, const msgCMP_ADDR1 &cmp) const
{
    int lo = 0;
    int hi = this->nElem;
    while (lo != hi) {
        int mid = (lo + hi) >> 1;
        int order = cmp.cmp(&this->pData[mid], key);
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
