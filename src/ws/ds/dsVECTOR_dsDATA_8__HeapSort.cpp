#include "dsVECTOR.h"
#include "dsDATA.h"
#include "dsSWAP.h"
#include "dsREF_VECTOR.h"

// dsVECTOR<dsDATA,8>::HeapSort<dsREF_VECTOR::SortCmp,dsSWAP<dsDATA>> @ 0x8261BD90
// Heap-sort fallback used by Sort for vectors larger than 100 elements. Three phases: build a
// 1-based max-heap over the storage (via Sisp sift-downs), repeatedly swap the root to the
// shrinking tail (full 3-way StoreValue exchange through a stack scratch, since dsDATA is not
// trivially swappable), then run a verify-and-selection-sort cleanup that re-sorts the whole
// vector if any inversion survives (identical in shape to Sort's small-n path; on a correct
// heap-sort it finds no inversion on the first scan and returns immediately).
template<>
template<>
void dsVECTOR<dsDATA, 8>::HeapSort<dsREF_VECTOR::SortCmp, dsSWAP<dsDATA>>(
        const dsREF_VECTOR::SortCmp &cmp, const dsSWAP<dsDATA> &swap)
{
    dsDATA *heap = this->pData - 1; // 1-based view: heap[1] == pData[0]
    int heapSize = this->nElem;

    // Phase 1 — build a max-heap by sifting every node from the last down to the root.
    for (int node = heapSize; node > 0; --node)
        this->Sisp(node, this->nElem, heap, cmp, swap);

    // Phase 2 — repeatedly move the max (root) to the end of the shrinking heap.
    if (heapSize > 1) {
        do {
            --heapSize;
            dsDATA *root = &heap[1];
            dsDATA *tail = &heap[heapSize + 1];

            dsDATA scratch;
            scratch.type = nullptr;
            scratch.StoreValue(*root);
            root->StoreValue(*tail);
            tail->StoreValue(scratch);
            if (scratch.type)
                scratch.type->Destroy((unsigned int *)&scratch);

            this->Sisp(1, heapSize, heap, cmp, swap);
        } while (heapSize > 1);
    }

    // Phase 3 — verify the result is fully ascending; on the first inversion, selection-sort it all.
    if (this->nElem > 1) {
        int scanned = 1;
        int probe = 0;
        while (true) {
            if (cmp.cmp(this->pData[probe], this->pData[probe + 1]) > 0)
                break; // inversion: pData[probe] > pData[probe+1]
            if (++scanned >= this->nElem)
                return; // fully ascending — nothing to fix up
            ++probe;
        }

        int nElem = this->nElem;
        if (nElem - 1 > 0) {
            for (int fillPos = 0; fillPos < nElem - 1; ++fillPos) {
                for (int scanIdx = fillPos + 1; scanIdx < nElem; ++scanIdx) {
                    if (cmp.cmp(this->pData[scanIdx], this->pData[fillPos]) < 0) {
                        dsDATA scratch;
                        scratch.type = nullptr;
                        scratch.StoreValue(this->pData[fillPos]);
                        this->pData[fillPos].StoreValue(this->pData[scanIdx]);
                        this->pData[scanIdx].StoreValue(scratch);
                        if (scratch.type)
                            scratch.type->Destroy((unsigned int *)&scratch);
                    }
                    nElem = this->nElem; // reloaded every inner iteration (matches the disasm)
                }
            }
        }
    }
}
