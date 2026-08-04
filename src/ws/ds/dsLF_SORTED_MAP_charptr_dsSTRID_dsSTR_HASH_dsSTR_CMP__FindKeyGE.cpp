#include "dsLF_SORTED_MAP.h"
#include "dsSTR_HASH.h"
#include "dsSTRID.h"

struct dsSTR_CMP; // boundary — const char* ordering policy

// dsLF_SORTED_MAP<const char*,dsSTRID,dsSTR_HASH,dsSTR_CMP>::FindKeyGE @ 0x82528648
// Lower-bound search of one ascending-sorted chain node: return the index of the first item whose
// key is >= *key (byte-wise strcmp). Empty/null node -> 0. On an exact match, scans left so the
// FIRST of any equal run is returned.
template<>
int dsLF_SORTED_MAP<const char *, dsSTRID, dsSTR_HASH, dsSTR_CMP>::FindKeyGE(
    const LINE *line, const char *const *key) const
{
    if (!line)
        return 0;
    int hi = line->length;
    int lo = 0;
    if (!line->length)
        return lo;

    const unsigned char *searchKey = (const unsigned char *)*key;
    int mid;
    while (true) {
        const unsigned char *keyCursor = searchKey;
        mid = (hi + lo) >> 1;
        const unsigned char *itemChars = (const unsigned char *)line->items[mid].key;
        int diff;
        do {
            int itemChar = *itemChars;
            diff = itemChar - *keyCursor;
            if (itemChar == 0)
                break;
            ++itemChars;
            ++keyCursor;
        } while (!diff);

        if (diff > 0) {        // item key > search key
            hi = mid;
        } else if (diff < 0) { // item key < search key
            lo = mid + 1;
        } else {
            break;             // exact match at mid
        }
        if (lo == hi)
            return lo;
    }

    // exact match: walk left to the first equal item
    while (mid > 0) {
        const unsigned char *itemChars = (const unsigned char *)line->items[mid - 1].key;
        const unsigned char *keyCursor = searchKey;
        int diff;
        do {
            int itemChar = *itemChars;
            diff = itemChar - *keyCursor;
            if (itemChar == 0)
                break;
            ++itemChars;
            ++keyCursor;
        } while (!diff);
        if (diff)
            break;
        --mid;
    }
    return mid;
}
