#include "dsVECTOR.h"
#include "dsTSTRING.h"
#include "dsSWAP.h"

struct dsCMP {}; // boundary — stateless ascending comparator; the string compare is inlined below

// dsVECTOR<dsTSTRING<char>,8>::HeapSort<dsCMP,dsSWAP<dsTSTRING<char>>> @ 0x82664220
// Heap-sort fallback for large vectors. `base` is the 1-based array (pData - 1, so base[1] ==
// pData[0], matching Sisp's 1-based indexing convention). Phase 1: build a max-heap by sifting
// down every node from nElem down to 1. Phase 2: repeatedly swap the heap root (base[1]) with the
// current last live element and sift-down the shrinking heap, extracting elements in ascending
// order. Phase 3: a verify-and-selection-sort cleanup pass — scans for the first adjacent
// out-of-order pair and, if the whole range already reads sorted, returns early; otherwise falls
// back to the same O(n^2) swap-forward selection sort used by Sort() for small vectors.
template<>
template<>
void dsVECTOR<dsTSTRING<char>, 8>::HeapSort<dsCMP, dsSWAP<dsTSTRING<char>>>(
        const dsCMP &cmp, const dsSWAP<dsTSTRING<char>> &swap)
{
    int nElem = this->nElem;
    dsTSTRING<char> *base = this->pData - 1; // 1-based array, per Sisp's convention

    // Phase 1: build the max-heap.
    for (int i = nElem; i > 0; --i)
        this->Sisp<dsCMP, dsSWAP<dsTSTRING<char>>>(i, this->nElem, base, cmp, swap);

    // Phase 2: repeatedly extract the root to the shrinking tail.
    if (nElem > 1) {
        int heapEnd = nElem;
        for (dsTSTRING<char> *tail = &base[nElem]; heapEnd > 1; --tail) {
            dsSWAP<dsTSTRING<char>>()(base[1], *tail);
            --heapEnd;
            this->Sisp<dsCMP, dsSWAP<dsTSTRING<char>>>(1, heapEnd, base, cmp, swap);
        }
    }

    // Phase 3: verify the result reads ascending; bail out early if so.
    int count = this->nElem;
    if (count <= 1)
        return;

    dsTSTRING<char> *pData = this->pData;
    int verifiedCount = 1;
    for (int i = 0; ; ++i) {
        const char *aChar = pData[i].pBuffer->str;
        const char *bChar = pData[i + 1].pBuffer->str;
        int forwardDiff;
        do {
            int aByte = (unsigned char)*aChar;
            forwardDiff = aByte - (unsigned char)*bChar;
            if (aByte == 0)
                break;
            ++aChar;
            ++bChar;
        } while (!forwardDiff);

        if (forwardDiff >= 0) {
            const char *aChar2 = pData[i].pBuffer->str;
            const char *bChar2 = pData[i + 1].pBuffer->str;
            int backwardDiff;
            do {
                int bByte = (unsigned char)*bChar2;
                backwardDiff = bByte - (unsigned char)*aChar2;
                if (bByte == 0)
                    break;
                ++bChar2;
                ++aChar2;
            } while (!backwardDiff);
            if ((unsigned int)backwardDiff >> 31 == 1)
                break; // out of order: fall through to the cleanup sort below
        }
        if (++verifiedCount >= count)
            return; // the whole range already reads ascending
    }

    // Cleanup: same O(n^2) swap-forward selection sort as Sort() for small vectors.
    for (int i = 0; i < count - 1; ++i) {
        for (int j = i + 1; j < count; ++j) {
            dsTSTRING<char> *slotI = &this->pData[i];
            dsTSTRING<char> *slotJ = &this->pData[j];

            const char *iChar = slotI->pBuffer->str;
            const char *jChar = slotJ->pBuffer->str;
            int forwardDiff;
            do {
                int jByte = (unsigned char)*jChar;
                forwardDiff = jByte - (unsigned char)*iChar;
                if (jByte == 0)
                    break;
                ++jChar;
                ++iChar;
            } while (!forwardDiff);

            if (forwardDiff < 0) {
                dsSWAP<dsTSTRING<char>>()(*slotI, *slotJ);
            } else {
                const char *iChar2 = slotJ->pBuffer->str;
                const char *jChar2 = slotI->pBuffer->str;
                int backwardDiff;
                do {
                    int iByte = (unsigned char)*jChar2;
                    backwardDiff = iByte - (unsigned char)*iChar2;
                    if (iByte == 0)
                        break;
                    ++jChar2;
                    ++iChar2;
                } while (!backwardDiff);
            }
            count = this->nElem;
        }
    }
}
