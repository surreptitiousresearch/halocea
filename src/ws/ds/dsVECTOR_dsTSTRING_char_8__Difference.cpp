#include "dsVECTOR.h"
#include "dsTSTRING.h"

struct dsCMP {}; // boundary — stateless ascending comparator; the string compare is inlined below

// dsVECTOR<dsTSTRING<char>,8>::Difference<dsCMP> @ 0x8263ABB8
// Symmetric set difference of two CMP-ascending-sorted vectors: append every element present in
// exactly one of `arr1`/`arr2` (sorted-merge two-pointer scan; ties advance both without
// appending). Any leftover tail of either vector past the other's end is appended wholesale.
template<>
template<>
void dsVECTOR<dsTSTRING<char>, 8>::Difference<dsCMP>(
        const dsVECTOR<dsTSTRING<char>, 8> &arr1, const dsVECTOR<dsTSTRING<char>, 8> &arr2, const dsCMP &)
{
    const dsTSTRING<char> *cursor1 = arr1.pData;
    const dsTSTRING<char> *cursor2 = arr2.pData;
    const dsTSTRING<char> *end1 = &arr1.pData[arr1.nElem];
    const dsTSTRING<char> *end2 = &arr2.pData[arr2.nElem];

    while (cursor1 < end1 && cursor2 < end2) {
        // dsCMP::cmp(cursor1, cursor2): three-way lexical compare (-1/0/1).
        const char *char1 = cursor1->pBuffer->str;
        const char *char2 = cursor2->pBuffer->str;
        int forwardDiff;
        do {
            int byte1 = (unsigned char)*char1;
            forwardDiff = byte1 - (unsigned char)*char2;
            if (byte1 == 0)
                break;
            ++char1;
            ++char2;
        } while (!forwardDiff);

        int cmpResult;
        if (forwardDiff < 0) {
            cmpResult = -1;
        } else {
            const char *char1b = cursor1->pBuffer->str;
            const char *char2b = cursor2->pBuffer->str;
            int backwardDiff;
            do {
                int byte2 = (unsigned char)*char2b;
                backwardDiff = byte2 - (unsigned char)*char1b;
                if (byte2 == 0)
                    break;
                ++char2b;
                ++char1b;
            } while (!backwardDiff);
            cmpResult = (unsigned int)backwardDiff >> 31;
        }

        if (cmpResult <= 0) {
            if (cmpResult >= 0) {
                // equal: present in both, skip.
                ++cursor1;
                ++cursor2;
            } else {
                this->PushBack(*cursor1); // only in arr1
                ++cursor1;
            }
        } else {
            this->PushBack(*cursor2); // only in arr2
            ++cursor2;
        }
    }
    while (cursor1 < end1)
        this->PushBack(*cursor1++);
    while (cursor2 < end2)
        this->PushBack(*cursor2++);
}
