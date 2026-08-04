#include "dsVECTOR.h"

#include "saLIST.h"

// boundary — sa comparator: orders a saLIST element against a target integer index.
struct msgCMP_INDEX {
    int cmp(const saLIST *elem, int key) const; // boundary
};

// dsVECTOR<saLIST,8>::FindSorted<msgCMP_INDEX,int> @ 0x82AC5AB8
// Binary search of the ascending-sorted vector for the element matching index `key`; returns its
// index or -1. const method (QBA mangle).
template<>
template<>
int dsVECTOR<saLIST, 8>::FindSorted<msgCMP_INDEX, int>(const int &key, const msgCMP_INDEX &cmp) const
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
