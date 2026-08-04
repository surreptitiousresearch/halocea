#include "dsVECTOR.h"
#include "dsDATA.h"
#include "dsSWAP.h"
#include "dsREF_VECTOR.h"

// dsVECTOR<dsDATA,8>::Sisp<dsREF_VECTOR::SortCmp,dsSWAP<dsDATA>> @ 0x8261BBD8
// Heap sift-down helper backing HeapSort. `array[p..q]` is a 1-based binary max-heap keyed by
// `cmp`. Sinks array[p]'s value down toward the leaves (picking, at each level, the child ordered
// greater by cmp), promoting each hole's child value up via StoreValue (dsDATA is not trivially
// relocatable, unlike the lighter instantiations that only permute an index field) and finally
// storing the saved root value into the resting hole.
template<>
template<>
void dsVECTOR<dsDATA, 8>::Sisp<dsREF_VECTOR::SortCmp, dsSWAP<dsDATA>>(
        int p, int q, dsDATA *array, const dsREF_VECTOR::SortCmp &cmp, const dsSWAP<dsDATA> &swap)
{
    int child = 2 * p;   // left child of the root
    int hole = p;        // index whose value is being sunk

    dsDATA saved;         // the root's value, carried down
    saved.type = nullptr;
    saved.StoreValue(array[p]);

    bool atBottom = child > q;
    if (child < q) {
        // both children exist: pick the one ordered greater by cmp
        if (cmp.cmp(array[child], array[child + 1]) < 0)
            ++child;
        atBottom = child > q;
    }

    if (!atBottom) {
        do {
            if (cmp.cmp(saved, array[child]) >= 0)
                break; // the saved value already dominates its largest child

            int prevHole = hole;
            hole = child;
            array[prevHole].StoreValue(array[child]); // promote the larger child

            child *= 2;
            atBottom = child > q;
            if (child < q) {
                if (cmp.cmp(array[child], array[child + 1]) < 0)
                    ++child;
                atBottom = child > q;
            }
        } while (!atBottom);
    }

    array[hole].StoreValue(saved);
    if (saved.type)
        saved.type->Destroy((unsigned int *)&saved);
}
