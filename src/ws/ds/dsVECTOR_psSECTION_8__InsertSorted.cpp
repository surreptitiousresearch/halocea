#include "dsVECTOR.h"

#include "psSECTION.h"

// Stateless ordering comparator used at this call site. cmp(element, val) returns <0 / 0 / >0.
// TEMPLATE_CMP::cmp's body is not reversed here (boundary) — only its call-site usage is modeled.
struct TEMPLATE_CMP {
    static int cmp(const psSECTION &element, const psSECTION &val); // boundary
};

// dsVECTOR<psSECTION,8>::InsertSorted<TEMPLATE_CMP> @ 0x829115D0
// Binary-search insert into a vector kept ascending by TEMPLATE_CMP order (standard lower-bound
// search, matching the compiled nested-loop CFG). On a miss, splices `val` at the lower-bound
// index and returns it. On a match, `dup` selects the duplicate policy: REPLACE overwrites the
// matched slot via psSECTION::operator=, IGNORE returns the match index unchanged, RIGHT inserts
// `val` immediately after the match, and any other value returns -1.
template<>
template<>
int dsVECTOR<psSECTION, 8>::InsertSorted<TEMPLATE_CMP>(const psSECTION &val, const TEMPLATE_CMP &, INS_SORT dup)
{
    int lo = 0;
    int hi = this->nElem;
    int matchIndex = -1;
    while (lo < hi) {
        int mid = (lo + hi) >> 1;
        int cmpResult = TEMPLATE_CMP::cmp(this->pData[mid], val);
        if (cmpResult > 0) {
            hi = mid;
        } else if (cmpResult < 0) {
            lo = mid + 1;
        } else {
            matchIndex = mid;
            break;
        }
    }

    if (matchIndex < 0) {
        this->Insert(lo, &val, 1);
        return lo;
    }

    switch (dup) {
    case INS_DUP_REPLACE:
        this->pData[matchIndex] = val; // psSECTION::operator=
        return matchIndex;
    case INS_DUP_IGNORE:
        return matchIndex;
    case INS_DUP_RIGHT:
        this->Insert(matchIndex, &val, 1);
        return matchIndex;
    default:
        return -1;
    }
}
