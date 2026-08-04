#include "dsVECTOR.h"
#include "../ps/psSECTION_RECORD.h"

// dsVECTOR<psSECTION_RECORD,8>::Sisp<ORS_CMP,ORS_SWAP> @ 0x825192F8
// Heap sift-down helper backing HeapSort. `array[p..q]` is a 1-based binary max-heap keyed on the
// interned key id (dsSTRID::id pointer) of the record each `sortedIdx` points at within cmp.arr.
// Sinks array[p]'s value down toward the leaves, at each level picking the child with the larger
// key. ORS_SWAP is stateless; the element move is inlined (only the `sortedIdx` field is carried).
template<>
template<>
void dsVECTOR<psSECTION_RECORD, 8>::Sisp<ORS_CMP, ORS_SWAP>(
        int p, int q, psSECTION_RECORD *array, const ORS_CMP &cmp, const ORS_SWAP &swap)
{
    (void)swap; // stateless: the exchange is inlined below

    int child = 2 * p;                          // left child of the root
    int hole = p;                               // index whose value is being sunk
    int savedSortedIdx = array[p].sortedIdx;    // the root's value, carried down

    bool atBottom = child > q;
    if (child < q) {
        // both children exist: pick the one with the larger key
        const char *leftKey  = (*cmp.arr)[array[child].sortedIdx].keyStrId.id;
        const char *rightKey = (*cmp.arr)[array[child + 1].sortedIdx].keyStrId.id;
        if (leftKey < rightKey)
            ++child; // right child has the larger key
        atBottom = child > q;
    }

    if (!atBottom) {
        do {
            const char *childKey = (*cmp.arr)[array[child].sortedIdx].keyStrId.id;
            const char *savedKey = (*cmp.arr)[savedSortedIdx].keyStrId.id;
            if (savedKey >= childKey)
                break; // the saved value already dominates its largest child

            int prevHole = hole;
            hole = child;
            array[prevHole].sortedIdx = array[child].sortedIdx; // promote the larger child

            child *= 2;
            atBottom = child > q;
            if (child < q) {
                const char *leftKey  = (*cmp.arr)[array[child].sortedIdx].keyStrId.id;
                const char *rightKey = (*cmp.arr)[array[child + 1].sortedIdx].keyStrId.id;
                if (leftKey < rightKey)
                    ++child;
                atBottom = child > q;
            }
        } while (!atBottom);
    }

    array[hole].sortedIdx = savedSortedIdx;
}
