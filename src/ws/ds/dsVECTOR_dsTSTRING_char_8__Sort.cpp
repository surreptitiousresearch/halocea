#include "dsVECTOR.h"
#include "dsTSTRING.h"
#include "dsSWAP.h"

struct dsCMP {}; // boundary — stateless ascending comparator; the string compare is inlined below

// dsVECTOR<dsTSTRING<char>,8>::Sort<dsCMP,dsSWAP<dsTSTRING<char>>> @ 0x82664988
// In-place ascending sort. Up to 100 elements: O(n^2) pass that, for each position i, scans j from
// i+1 to the end and swaps whenever pData[j] < pData[i] (bubbling the smallest-seen value back to
// i as the scan proceeds — the compare is inlined two-way strcmp, matching the dsCMP idiom). Larger
// vectors delegate to HeapSort.
template<>
template<>
void dsVECTOR<dsTSTRING<char>, 8>::Sort<dsCMP, dsSWAP<dsTSTRING<char>>>(
        const dsCMP &cmp, const dsSWAP<dsTSTRING<char>> &swap)
{
    int nElem = this->nElem;
    if (nElem > 100) {
        this->HeapSort<dsCMP, dsSWAP<dsTSTRING<char>>>(cmp, swap);
        return;
    }

    for (int i = 0; i < nElem - 1; ++i) {
        for (int j = i + 1; j < nElem; ++j) {
            dsTSTRING<char> *slotI = &this->pData[i];
            dsTSTRING<char> *slotJ = &this->pData[j];

            // strcmp(slotI, slotJ): forward pass.
            const char *iChar = slotI->pBuffer->str;
            const char *jChar = slotJ->pBuffer->str;
            int forwardDiff;
            do {
                int jByte = (unsigned char)*jChar;
                forwardDiff = jByte - (unsigned char)*iChar; // NOTE: matches the disasm's operand order
                if (jByte == 0)
                    break;
                ++jChar;
                ++iChar;
            } while (!forwardDiff);

            if (forwardDiff < 0) {
                // slotJ < slotI: bring the smaller value into position i.
                dsSWAP<dsTSTRING<char>>()(*slotI, *slotJ);
            } else {
                // Backward pass performed by the disasm too, but its result is unused here
                // (the loop only acts on the forward-pass outcome).
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
            nElem = this->nElem;
        }
    }
}
