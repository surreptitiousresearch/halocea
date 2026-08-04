#include "dsVECTOR.h"
#include "dsTSTRING.h"

struct dsCMP {}; // boundary — stateless ascending comparator

// dsVECTOR<dsTSTRING<char>,8>::FindSorted<dsCMP,dsTSTRING<char>> @ 0x82602B88
// Binary search of an ascending-sorted vector for the element whose text equals `key`'s, via
// dsCMP::cmp<dsTSTRING<char>,dsTSTRING<char>> (three-way lexical compare, inlined below to match
// InsertSorted<dsCMP>'s style). Returns its index or -1.
template<>
template<>
int dsVECTOR<dsTSTRING<char>, 8>::FindSorted<dsCMP, dsTSTRING<char>>(
        const dsTSTRING<char> &key, const dsCMP &) const
{
    int lo = 0;
    int hi = this->nElem;
    while (lo != hi) {
        int mid = (lo + hi) >> 1;

        // dsCMP::cmp(pData[mid], key): three-way lexical compare (-1/0/1).
        const dsTSTRING<char> &elem = this->pData[mid];
        const char *elemChar = elem.pBuffer->str;
        const char *keyChar = key.pBuffer->str;
        int forwardDiff;
        do {
            int elemByte = (unsigned char)*elemChar;
            forwardDiff = elemByte - (unsigned char)*keyChar;
            if (elemByte == 0)
                break;
            ++elemChar;
            ++keyChar;
        } while (!forwardDiff);

        int cmpResult;
        if (forwardDiff < 0) {
            cmpResult = -1;
        } else {
            const char *elemChar2 = elem.pBuffer->str;
            const char *keyChar2 = key.pBuffer->str;
            int backwardDiff;
            do {
                int keyByte = (unsigned char)*keyChar2;
                backwardDiff = keyByte - (unsigned char)*elemChar2;
                if (keyByte == 0)
                    break;
                ++keyChar2;
                ++elemChar2;
            } while (!backwardDiff);
            cmpResult = (unsigned int)backwardDiff >> 31; // 1 if elem > key, else 0
        }

        if (cmpResult <= 0) {
            if (cmpResult >= 0)
                return mid; // equal match
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return -1;
}
