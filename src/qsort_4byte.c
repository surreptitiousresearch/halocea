/* qsort_4byte 0x8381A6E8 — in-place sort of an array of 4-byte ints using an iterative quicksort with an
 * explicit pending-partition stack and a selection-sort fallback for partitions of <= 8 elements. The
 * comparator returns nonzero when its first argument should sort before its second. Partitions are pushed
 * smaller-first so the explicit stack never exceeds ~log2(num) depth.
 *
 * DEVIATION: the decompiler modeled the two parallel partition-bound stacks as single BYREF ints (v19/v20);
 * the surrounding 0x80-byte stack frame shows each is really a 32-entry array of element pointers. */

/* compare slot respelled u8->int 2026-07-30: all three impls return width-agnostic 0/1 booleans
 * (li/srwi/clrlwi-31, no byte-normalize); u8 was the decompiler hint (C4133 at both structure_render sites) */
void qsort_4byte(int *base, unsigned int num, int (*compare)(int, int))
{
    int stack_depth;
    int *lo;
    int *hi;
    int **hi_stack;
    int **lo_stack;
    unsigned int span;
    int *max_elem;
    int *i;
    int tmp;
    unsigned int pivot_offset;
    int *left;
    int *right;
    int swap_tmp;
    int *hi_stack_storage[32];
    int *lo_stack_storage[32];

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

            pivot_offset = (2 * span) & ~0x3u; /* middle element, rounded down to 4-byte stride */
            left = lo;
            right = hi + 1;
            swap_tmp = *(int *)((char *)lo + pivot_offset);
            *(int *)((char *)lo + pivot_offset) = *lo;
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
