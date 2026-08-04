#include "dsVECTOR.h"
#include "../ap/apDATA_TRACKER.h"

struct SR_REC_PRED; // boundary — record predicate functor; comparison is inlined as a strcmp here.

// dsVECTOR<apDATA_TRACKER::RECORD,8>::InsertSorted<SR_REC_PRED> @ 0x82A25278
// Binary-search insert into a name-ascending record vector. On a miss, splice `val` at the
// lower-bound position and return it. On a name match, `dup` selects overwrite / ignore /
// insert-after (see dsVECTOR::INS_SORT); an unrecognized `dup` returns -1. The SR_REC_PRED
// comparator is a stateless functor whose byte-wise name compare is inlined below.
template<>
template<>
int dsVECTOR<apDATA_TRACKER::RECORD, 8>::InsertSorted<SR_REC_PRED>(
        const apDATA_TRACKER::RECORD &val, const SR_REC_PRED & /*cmp*/,
        dsVECTOR<apDATA_TRACKER::RECORD, 8>::INS_SORT dup)
{
    int hi = this->nElem;
    int lo = 0;
    if (!hi) {
        this->Insert(lo, &val, 1);
        return lo;
    }

    apDATA_TRACKER::RECORD *records = this->pData;
    for (;;) {
        const char *keyChars = val.name.pBuffer->str;
        int mid = (hi + lo) >> 1;
        const char *recordChars = records[mid].name.pBuffer->str;
        int diff;
        do {
            int recordChar = (unsigned char)*recordChars;
            diff = recordChar - (unsigned char)*keyChars;
            if (recordChar == 0)
                break;
            ++recordChars;
            ++keyChars;
        } while (!diff);

        if (diff == 0) {                 // exact name match at mid
            if (dup == INS_DUP_REPLACE) {
                records[mid] = val;
                return mid;
            }
            if (dup == INS_DUP_IGNORE)
                return mid;
            if (dup != INS_DUP_RIGHT)
                return -1;
            this->Insert(mid, &val, 1);  // insert after the match ((hi+lo)>>1 == mid)
            return mid;
        }

        if (diff > 0)
            hi = mid;                    // key sorts before mid
        else
            lo = mid + 1;                // key sorts after mid

        if (lo == hi) {                  // converged on a gap — splice the new record in
            this->Insert(lo, &val, 1);
            return lo;
        }
    }
}
