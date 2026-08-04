#include "dsVECTOR.h"
#include "dsTSTRING.h"
#include <string.h>

struct dsSTRING_NO_CASE_CMP {}; // boundary — stateless case-insensitive ascending comparator

// dsVECTOR<dsTSTRING<char>,8>::InsertSorted<dsSTRING_NO_CASE_CMP> @ 0x82926588
// Binary-search insert into a vector kept ascending by case-insensitive lexical string order
// (stricmp, a single library call here rather than the dsCMP sibling's inlined double byte-scan).
// On a miss, splice `val` at the lower-bound index and return it. On a match, `dup` selects the
// duplicate policy (replace / ignore / duplicate-right; anything else returns -1).
// Deviation: the disasm implements this via a goto-heavy single loop with two exit points; folded
// here into the equivalent standard binary-search-with-early-equal-exit shape (verified equivalent
// control flow: loop while lo != hi, narrow hi on cmp>0, advance lo on cmp<0, break on cmp==0).
template<>
template<>
int dsVECTOR<dsTSTRING<char>, 8>::InsertSorted<dsSTRING_NO_CASE_CMP>(
        const dsTSTRING<char> &val, const dsSTRING_NO_CASE_CMP &, INS_SORT dup)
{
    int lo = 0;
    int hi = this->nElem;
    int mid = lo;
    int cmpResult = 0;

    while (lo != hi) {
        mid = (hi + lo) >> 1;
        cmpResult = stricmp(this->pData[mid].pBuffer->str, val.pBuffer->str);
        if (cmpResult > 0)
            hi = mid;
        else if (cmpResult < 0)
            lo = mid + 1;
        else
            break; // equal match at mid
    }

    if (lo == hi) {
        this->Insert(lo, &val, 1);
        return lo;
    }

    int matchIndex = mid;
    if (dup == INS_DUP_REPLACE) {
        this->pData[matchIndex] = val; // dsTSTRING<char>::operator= (share buffer)
        return matchIndex;
    }
    if (dup == INS_DUP_IGNORE)
        return matchIndex;
    if (dup != INS_DUP_RIGHT)
        return -1;
    this->Insert(matchIndex, &val, 1);
    return matchIndex;
}
