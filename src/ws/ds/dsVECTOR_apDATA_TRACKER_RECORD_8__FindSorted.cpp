#include "dsVECTOR.h"
#include "../ap/apDATA_TRACKER.h"

struct SR_REC_PRED; // boundary — record predicate functor; comparison is inlined as a strcmp here.

// dsVECTOR<apDATA_TRACKER::RECORD,8>::FindSorted<SR_REC_PRED,const char*> @ 0x82A24390
// Binary search of a name-ascending record vector for the record whose name equals *key
// (byte-wise strcmp on RECORD::name); returns its index, or -1 if absent. The SR_REC_PRED
// comparator is a stateless functor whose key extraction/compare is inlined below.
template<>
template<>
int dsVECTOR<apDATA_TRACKER::RECORD, 8>::FindSorted<SR_REC_PRED, const char *>(
    const char *const &key, const SR_REC_PRED & /*cmp*/) const
{
    int hi = this->nElem;
    int lo = 0;
    if (hi) {
        apDATA_TRACKER::RECORD *records = this->pData;
        do {
            const unsigned char *keyChars = (const unsigned char *)key;
            int mid = (hi + lo) >> 1;
            const char *recordChars = records[mid].name.pBuffer->str;
            int diff;
            do {
                int recordChar = (unsigned char)*recordChars;
                diff = recordChar - *keyChars;
                if (recordChar == 0)
                    break;
                ++recordChars;
                ++keyChars;
            } while (!diff);

            if (diff <= 0) {
                if (diff >= 0)
                    return mid;
                lo = mid + 1;
            } else {
                hi = mid;
            }
        } while (lo != hi);
    }
    return -1;
}
