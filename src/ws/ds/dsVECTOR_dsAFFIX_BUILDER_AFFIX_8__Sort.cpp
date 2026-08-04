#include "dsVECTOR.h"
#include "dsSWAP.h"
#include "dsAFFIX_BUILDER.h"

struct SRT_CMP; // boundary — stateless affix comparator; its ordering (by name) is inlined below.

// Byte-wise name compare matching the inlined comparator: walk `ref` and `test` in lock-step and
// return (test char - ref char) at the first difference, treating a NUL in `test` as the stop.
// A negative result means `test` sorts before `ref`.
static int AffixNameByteDiff(const char *ref, const char *test)
{
    int diff;
    do {
        int testChar = (unsigned char)*test;
        diff = testChar - (unsigned char)*ref;
        if (testChar == 0)
            break;
        ++test;
        ++ref;
    } while (!diff);
    return diff;
}

// dsVECTOR<dsAFFIX_BUILDER::AFFIX,8>::Sort<SRT_CMP,dsSWAP<AFFIX>> @ 0x82A78FA0
// In-place sort of the affix vector, ascending by affix name. For up to 100 elements this uses an
// O(n^2) selection-style bubble (each pass floats the smaller name toward the front); larger vectors
// delegate to HeapSort. SRT_CMP is a stateless functor whose name compare is inlined; dsSWAP<AFFIX>
// is a stateless functor whose whole-element exchange is inlined as a temp-copy plus two assignments
// (each moves the shared name-buffer reference and deep-copies the params vector). `nElem` is
// reloaded every inner iteration, matching the disasm.
template<>
template<>
void dsVECTOR<dsAFFIX_BUILDER::AFFIX, 8>::Sort<SRT_CMP, dsSWAP<dsAFFIX_BUILDER::AFFIX> >(
        const SRT_CMP &cmp, const dsSWAP<dsAFFIX_BUILDER::AFFIX> &swap)
{
    int nElem = this->nElem;
    if (nElem <= 100) {
        for (int i = 0; i < nElem - 1; ++i) {
            for (int j = i + 1; j < nElem; ++j) {
                const char *nameI = this->pData[i].name.pBuffer->str;
                const char *nameJ = this->pData[j].name.pBuffer->str;
                // diff < 0 when pData[j]'s name sorts before pData[i]'s -> exchange them.
                if (AffixNameByteDiff(nameI, nameJ) < 0) {
                    // dsSWAP<AFFIX>: exchange pData[i] and pData[j] (inlined temp-based swap).
                    dsAFFIX_BUILDER::AFFIX temp(this->pData[i]);
                    this->pData[i] = this->pData[j];
                    this->pData[j] = temp;
                }
                nElem = this->nElem;
            }
        }
    } else {
        this->HeapSort(cmp, swap);
    }
}
