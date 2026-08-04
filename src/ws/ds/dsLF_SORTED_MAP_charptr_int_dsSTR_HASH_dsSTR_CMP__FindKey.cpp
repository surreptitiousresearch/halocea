#include "dsLF_SORTED_MAP.h"

struct dsSTR_HASH; // boundary — const char* hashing policy
struct dsSTR_CMP;  // boundary — const char* ordering policy

// dsLF_SORTED_MAP<const char*,int,dsSTR_HASH,dsSTR_CMP>::FindKey @ 0x82613D18
// Binary search a single chain node's ascending-sorted items for *key (byte-wise strcmp on the
// item key); returns the item index within the node, or -1 (empty node / absent key).
template<>
int dsLF_SORTED_MAP<const char *, int, dsSTR_HASH, dsSTR_CMP>::FindKey(
    const LINE *line, const char *const *key) const
{
    if (line) {
        int hi = line->length;
        int lo = 0;
        if (line->length) {
            do {
                const unsigned char *keyChars = (const unsigned char *)*key;
                int mid = (hi + lo) >> 1;
                const char *itemChars = line->items[mid].key;
                int diff;
                do {
                    int itemChar = (unsigned char)*itemChars;
                    diff = itemChar - *keyChars;
                    if (itemChar == 0)
                        break;
                    ++itemChars;
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
    }
    return -1;
}
