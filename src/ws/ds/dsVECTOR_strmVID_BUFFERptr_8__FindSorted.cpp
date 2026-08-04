#include "dsVECTOR.h"

struct strmVID_BUFFER;       // boundary — strm subsystem video buffer (element is a pointer)
struct strmVID_BUF_INS_PRED; // boundary — ordering predicate (compares by pointer value here)

// dsVECTOR<strmVID_BUFFER*,8>::FindSorted<strmVID_BUF_INS_PRED,strmVID_BUFFER*> @ 0x82698338
// Binary search a pointer-ascending-sorted vector for the element equal to `key` (the predicate
// reduces to a raw pointer comparison in the compiled code); returns its index, or -1 if absent.
template<>
template<>
int dsVECTOR<strmVID_BUFFER *, 8>::FindSorted<strmVID_BUF_INS_PRED, strmVID_BUFFER *>(
    strmVID_BUFFER *const &key, const strmVID_BUF_INS_PRED &) const
{
    int hi = this->nElem;
    int lo = 0;
    if (hi) {
        do {
            int mid = (hi + lo) >> 1;
            strmVID_BUFFER *candidate = this->pData[mid];
            if (candidate < key) {
                lo = mid + 1;
            } else if (key >= candidate) { // candidate >= key and key >= candidate => equal match
                return mid;
            } else {
                hi = mid;
            }
        } while (lo != hi);
    }
    return -1;
}
