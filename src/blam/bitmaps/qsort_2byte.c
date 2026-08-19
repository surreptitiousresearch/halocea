/* qsort_2byte @0x8381A508 */
/* qsort_2byte 0x8381A508 — in-place sort of an array of 2-byte (__int16) elements using an iterative quicksort
 * with an explicit pending-partition stack and a selection-sort fallback for partitions of <= 8 elements. The
 * comparator returns nonzero when its first argument should sort before its second. Partitions are pushed
 * smaller-first so the explicit stack never exceeds ~log2(num) depth. Sibling of qsort_4byte (identical
 * structure, __int16 stride and a byte pivot offset of span & ~1).
 *
 * DEVIATION: the decompiler modeled the two parallel partition-bound stacks as single BYREF ints (v19/v20);
 * the surrounding stack frame shows each is really a 32-entry array of element pointers. */
#include <stdint.h>

void qsort_2byte(int16_t *base, unsigned int num, uint8_t (*compare)(int16_t, int16_t))
{
    int stack_depth;
    int16_t *lo;
    int16_t *hi;
    int16_t **hi_stack;
    int16_t **lo_stack;
    unsigned int span;
    int16_t *max_elem;
    int16_t *i;
    int16_t tmp;
    unsigned int pivot_offset;
    int16_t *left;
    int16_t *right;
    int16_t swap_tmp;
    int16_t *hi_stack_storage[32];
    int16_t *lo_stack_storage[32];

    if ( num < 2 )
        return;

    stack_depth = 0;
    lo = base;
    hi = &base[num - 1];
    hi_stack = hi_stack_storage;
    lo_stack = lo_stack_storage;

    while ( 1 )
    {
        while ( 1 )
        {
            span = hi - lo + 1;
            if ( span <= 8 )
            {
                for ( ; hi > lo; --hi )
                {
                    max_elem = lo;
                    for ( i = lo + 1; i <= hi; ++i )
                    {
                        if ( compare(*i, *max_elem) )
                            max_elem = i;
                    }
                    tmp = *max_elem;
                    *max_elem = *hi;
                    *hi = tmp;
                }
                goto pop_partition;
            }

            pivot_offset = span & ~0x1u;   /* round byte span down to a whole 2-byte element */
            left = lo;
            right = hi + 1;
            swap_tmp = *(int16_t *)((char *)lo + pivot_offset);
            *(int16_t *)((char *)lo + pivot_offset) = *lo;
            *lo = swap_tmp;

            while ( 1 )
            {
                do
                    ++left;
                while ( left <= hi && !compare(*left, *lo) );
                do
                    --right;
                while ( right > lo && compare(*right, *lo) );
                if ( right < left )
                    break;
                swap_tmp = *left;
                *left = *right;
                *right = swap_tmp;
            }

            swap_tmp = *lo;
            *lo = *right;
            *right = swap_tmp;

            if ( (char *)right - (char *)lo - 1 >= (char *)hi - (char *)left )
                break;

            if ( left < hi )
            {
                *++lo_stack = left;
                ++stack_depth;
                *++hi_stack = hi;
            }
            if ( lo + 1 >= right )
            {
pop_partition:
                --stack_depth;
                --lo_stack;
                --hi_stack;
                if ( stack_depth < 0 )
                    return;
                lo = lo_stack[1];
                hi = hi_stack[1];
            }
            else
            {
                hi = right - 1;
            }
        }

        if ( lo + 1 < right )
        {
            lo_stack[1] = lo;
            ++stack_depth;
            hi_stack[1] = right - 1;
            ++lo_stack;
            ++hi_stack;
        }
        if ( left >= hi )
            goto pop_partition;
        lo = left;
    }
}
