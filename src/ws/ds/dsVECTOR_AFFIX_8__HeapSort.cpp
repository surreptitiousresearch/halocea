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

// dsVECTOR<dsAFFIX_BUILDER::AFFIX,8>::HeapSort<SRT_CMP,dsSWAP<AFFIX>> @ 0x82A788E0
// Heap-sort fallback used by Sort for affix vectors larger than 100 elements, ascending by affix
// name. Three phases: build a 1-based max-heap (via Sisp sift-downs), repeatedly swap the root to
// the shrinking tail, then verify-and-selection-sort as a cleanup. SRT_CMP's name compare is
// inlined (AffixNameByteDiff); dsSWAP<AFFIX>'s whole-element exchange is inlined as a temp-copy plus
// two assignments (each moves the shared name-buffer reference and deep-copies the params vector).
//
// CAVEAT: the trailing cleanup (phase 3) is the same O(n^2) selection sort as Sort()'s small-n path.
// On a correct heap-sort the first scan finds no inversion and returns immediately; it is reproduced
// verbatim from the compiled body as a defensive fallback, not an inferred addition.
template<>
template<>
void dsVECTOR<dsAFFIX_BUILDER::AFFIX, 8>::HeapSort<SRT_CMP, dsSWAP<dsAFFIX_BUILDER::AFFIX> >(
        const SRT_CMP &cmp, const dsSWAP<dsAFFIX_BUILDER::AFFIX> &swap)
{
    dsAFFIX_BUILDER::AFFIX *heap = this->pData - 1; // 1-based view: heap[1] == pData[0]
    int heapSize = this->nElem;

    // Phase 1 — build a max-heap by sifting every node from the last down to the root.
    for (int node = heapSize; node > 0; --node)
        this->Sisp(node, this->nElem, heap, cmp, swap);

    // Phase 2 — repeatedly move the max (root) to the end of the shrinking heap.
    if (heapSize > 1) {
        do {
            // dsSWAP<AFFIX>: exchange the root with the current tail (inlined temp-based swap).
            dsAFFIX_BUILDER::AFFIX temp(heap[1]);
            heap[1] = heap[heapSize];
            heap[heapSize] = temp;
            --heapSize;
            this->Sisp(1, heapSize, heap, cmp, swap);
        } while (heapSize > 1);
    }

    // Phase 3 — verify the result is fully ascending; on the first inversion, selection-sort it all.
    if (this->nElem > 1) {
        int scanned = 1;
        int probe = 0;
        while (true) {
            const char *curName = this->pData[probe].name.pBuffer->str;
            const char *nextName = this->pData[probe + 1].name.pBuffer->str;
            // > 0 when pData[probe]'s name sorts after pData[probe+1]'s -> an inversion.
            if (AffixNameByteDiff(nextName, curName) > 0)
                break;
            if (++scanned >= this->nElem)
                return; // fully ascending — nothing to fix up
            ++probe;
        }

        int nElem = this->nElem;
        for (int fillPos = 0; fillPos < nElem - 1; ++fillPos) {
            for (int scanIdx = fillPos + 1; scanIdx < nElem; ++scanIdx) {
                const char *fillName = this->pData[fillPos].name.pBuffer->str;
                const char *scanName = this->pData[scanIdx].name.pBuffer->str;
                // diff < 0 when pData[scanIdx]'s name sorts before pData[fillPos]'s -> exchange them.
                if (AffixNameByteDiff(fillName, scanName) < 0) {
                    // dsSWAP<AFFIX>: exchange the two records (inlined temp-based swap).
                    dsAFFIX_BUILDER::AFFIX temp(this->pData[fillPos]);
                    this->pData[fillPos] = this->pData[scanIdx];
                    this->pData[scanIdx] = temp;
                }
                nElem = this->nElem; // reloaded every inner iteration (matches the disasm)
            }
        }
    }
}
