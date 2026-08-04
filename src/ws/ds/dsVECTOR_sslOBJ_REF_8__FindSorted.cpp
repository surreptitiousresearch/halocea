#include "dsVECTOR.h"
#include "../ssl/sslOBJ_REF.h"
#include "dsTSTRING.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

struct sslCMP_NAMES {}; // boundary — comparator over object names (byte-wise strcmp, inlined below)

// dsVECTOR<sslOBJ_REF,8>::FindSorted<sslCMP_NAMES,dsTSTRING<char>> @ 0x825CB258
// Binary search of the ascending-sorted ref vector for the element whose object name compares
// equal to `key`. Each probe resolves the element's name via sslOBJ_REF::GetName() (returned by
// value, so its temporary buffer is released after the compare) and compares it byte-wise against
// `key`. Returns the matching index, or -1.
// CAVEAT: the decompiler mistyped the GetName sret temporary as an sslOBJ_REF and its buffer
// pointer as an sslOBJECT* with a refCount; both are really the returned dsTSTRING<char> and its
// buffer header. Reconstructed as the intended by-value string temporary.
template<>
template<>
int dsVECTOR<sslOBJ_REF, 8>::FindSorted<sslCMP_NAMES, dsTSTRING<char> >(
        const dsTSTRING<char> &key, const sslCMP_NAMES &cmp) const
{
    (void)cmp;
    int hi = this->nElem;
    int lo = 0;
    if (!hi)
        return -1;

    for (;;) {
        int mid = (hi + lo) >> 1;
        dsTSTRING<char> elemName = this->pData[mid].GetName();

        // Forward compare: sign of (elemName - key) at the first differing byte.
        const char *namePtr = elemName.pBuffer->str;
        const char *keyPtr = key.pBuffer->str;
        int forwardDiff;
        for (;;) {
            int nameCh = (unsigned char)*namePtr;
            forwardDiff = nameCh - (unsigned char)*keyPtr;
            if (nameCh == 0)
                break;
            ++namePtr;
            ++keyPtr;
            if (forwardDiff != 0)
                break;
        }

        int order;
        if (forwardDiff >= 0) {
            // Backward compare: sign of (key - elemName); order is -1 when key precedes elemName.
            const char *namePtr2 = elemName.pBuffer->str;
            const char *keyPtr2 = key.pBuffer->str;
            unsigned int backDiff;
            for (;;) {
                int keyCh = (unsigned char)*keyPtr2;
                backDiff = keyCh - (unsigned int)(unsigned char)*namePtr2;
                if (keyCh == 0)
                    break;
                ++keyPtr2;
                ++namePtr2;
                if (backDiff != 0)
                    break;
            }
            order = (int)backDiff >> 31;
        } else {
            order = -1;
        }

        // Release the temporary name string's buffer reference.
        dsTSTRING_BUF_HEADER<char> *buffer = elemName.pBuffer;
        if (--buffer->refCount == 0)
            dlFree(buffer);

        if (order <= 0) {
            if (order < 0) {
                lo = mid + 1;
                if (lo == hi)
                    return -1;
                continue;
            }
            return mid; // order == 0: names match
        }
        hi = mid;
        if (lo == hi)
            return -1;
    }
}
