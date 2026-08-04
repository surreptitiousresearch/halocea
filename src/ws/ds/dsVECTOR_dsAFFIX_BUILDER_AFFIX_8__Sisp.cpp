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

// dsVECTOR<dsAFFIX_BUILDER::AFFIX,8>::Sisp<SRT_CMP,dsSWAP<AFFIX>> @ 0x82A77DD8  (protected)
// Heap sift-down helper backing HeapSort. `array[p..q]` is a 1-based binary max-heap keyed on the
// affix name. Save array[p]'s value, then sink it toward the leaves: at each level pick the child
// with the larger name and, while the saved value is smaller than that child, promote the child up
// into the hole; finally drop the saved value into the resulting hole. Both SRT_CMP (name compare)
// and dsSWAP<AFFIX> (element move) are stateless; the element carries via AFFIX copy-construct and
// assignment (shared name-buffer reference + deep-copied params vector).
template<>
template<>
void dsVECTOR<dsAFFIX_BUILDER::AFFIX, 8>::Sisp<SRT_CMP, dsSWAP<dsAFFIX_BUILDER::AFFIX> >(
        int p, int q, dsAFFIX_BUILDER::AFFIX *array, const SRT_CMP & /*cmp*/,
        const dsSWAP<dsAFFIX_BUILDER::AFFIX> & /*swap*/)
{
    int hole = p;
    int child = 2 * p;                          // left child of the root
    dsAFFIX_BUILDER::AFFIX saved(array[p]);      // the root's value, carried down

    // Pick the larger of the root's two children (if both exist).
    if (child < q) {
        // diff < 0 when array[child] sorts before array[child+1] -> the right child is larger.
        if (AffixNameByteDiff(array[child + 1].name.pBuffer->str,
                              array[child].name.pBuffer->str) < 0)
            ++child;
    }

    if (child <= q) {
        for (;;) {
            // Stop once the saved value dominates its largest child.
            // diff >= 0 here means saved's name is >= array[child]'s name.
            if (AffixNameByteDiff(array[child].name.pBuffer->str,
                                  saved.name.pBuffer->str) >= 0)
                break;

            array[hole] = array[child];         // promote the larger child into the hole
            hole = child;
            child *= 2;
            if (child < q) {
                if (AffixNameByteDiff(array[child + 1].name.pBuffer->str,
                                      array[child].name.pBuffer->str) < 0)
                    ++child;
            }
            if (child > q)
                break;
        }
    }

    array[hole] = saved;                        // settle the saved value into place
}
