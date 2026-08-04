#include "dsVECTOR.h"
#include "../ds/dsSTRID.h"
#include "../ps/psSECTION.h"

// Stateless ordering comparator used at this call site. cmp(element, key) returns <0 / 0 / >0.
// TEMPLATE_CMP::cmp's body is not reversed here (boundary) — only its call-site usage is modeled.
struct TEMPLATE_CMP {
    static int cmp(const psSECTION &element, dsSTRID key); // boundary
};

// dsVECTOR<psSECTION,8>::FindSorted<TEMPLATE_CMP,dsSTRID> @ 0x82906AF8
// Binary search of an ascending-sorted vector for the element matching `key`'s ordering; returns
// its index or -1. `key` is passed to TEMPLATE_CMP::cmp by value (dsSTRID is a single 4-byte
// pointer field, so this is the by-value dsSTRID the disassembly loads from `key->id`).
template<>
template<>
int dsVECTOR<psSECTION, 8>::FindSorted<TEMPLATE_CMP, dsSTRID>(const dsSTRID &key, const TEMPLATE_CMP &) const
{
    int lo = 0;
    int hi = this->nElem;
    while (lo != hi) {
        int mid = (lo + hi) >> 1;
        int cmpResult = TEMPLATE_CMP::cmp(this->pData[mid], key);
        if (cmpResult <= 0) {
            if (cmpResult >= 0)
                return mid;
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return -1;
}
