#include "dsVECTOR.h"
#include "../ps/psSECTION_RECORD.h"

// dsVECTOR<psSECTION_RECORD,8>::HeapSort<ORS_CMP,ORS_SWAP> @ 0x82519D58
// Heap-sort fallback used by Sort for vectors larger than 100 records. Like Sort/Sisp, it orders the
// vector's `sortedIdx` permutation by the interned key id (dsSTRID::id pointer) of the record each
// slot points at within cmp.arr; only the `sortedIdx` field is ever moved. The routine has three
// phases: build a 1-based max-heap, repeatedly swap the root to the shrinking tail, then run a
// verify-and-selection-sort cleanup that re-sorts the whole vector if any inversion survives.
//
// CAVEAT: the trailing cleanup (phase 3) is an O(n^2) selection sort identical to Sort()'s small-n
// path. On a correct heap-sort it finds no inversion on the first scan and returns immediately; it is
// reproduced verbatim from the compiled body as a defensive fallback, not an inferred addition.
template<>
template<>
void dsVECTOR<psSECTION_RECORD, 8>::HeapSort<ORS_CMP, ORS_SWAP>(
        const ORS_CMP &cmp, const ORS_SWAP &swap)
{
    (void)swap; // stateless: the sortedIdx exchange is inlined below

    psSECTION_RECORD *heap = this->pData - 1; // 1-based view: heap[1] == pData[0]
    int heapSize = this->nElem;

    // Phase 1 — build a max-heap by sifting every node from the last down to the root.
    for (int node = heapSize; node > 0; --node)
        this->Sisp(node, this->nElem, heap, cmp, swap);

    // Phase 2 — repeatedly move the max (root) to the end of the shrinking heap.
    if (heapSize > 1) {
        do {
            --heapSize;
            int rootSortedIdx = heap[1].sortedIdx;
            heap[1].sortedIdx = heap[heapSize + 1].sortedIdx;
            heap[heapSize + 1].sortedIdx = rootSortedIdx;
            this->Sisp(1, heapSize, heap, cmp, swap);
        } while (heapSize > 1);
    }

    // Phase 3 — verify the result is fully ascending; on the first inversion, selection-sort it all.
    if (this->nElem > 1) {
        int scanned = 1;
        int probe = 0;
        while (true) {
            const psSECTION_RECORD *nextRec = &(*cmp.arr)[this->pData[probe + 1].sortedIdx];
            const psSECTION_RECORD *curRec = &(*cmp.arr)[this->pData[probe].sortedIdx];
            if (nextRec->keyStrId.id < curRec->keyStrId.id)
                break; // inversion: pData[probe] > pData[probe+1]
            if (++scanned >= this->nElem)
                return; // fully ascending — nothing to fix up
            ++probe;
        }

        int nElem = this->nElem;
        if (nElem - 1 > 0) {
            for (int fillPos = 0; fillPos < nElem - 1; ++fillPos) {
                for (int scanIdx = fillPos + 1; scanIdx < nElem; ++scanIdx) {
                    int fillSortedIdx = this->pData[fillPos].sortedIdx;
                    int scanSortedIdx = this->pData[scanIdx].sortedIdx;
                    // ORS_CMP: order by the key id of the record each sortedIdx points at.
                    if ((*cmp.arr)[scanSortedIdx].keyStrId.id < (*cmp.arr)[fillSortedIdx].keyStrId.id) {
                        // ORS_SWAP: exchange the two records' sortedIdx fields only.
                        this->pData[fillPos].sortedIdx = scanSortedIdx;
                        this->pData[scanIdx].sortedIdx = fillSortedIdx;
                    }
                    nElem = this->nElem; // reloaded every inner iteration (matches the disasm)
                }
            }
        }
    }
}
