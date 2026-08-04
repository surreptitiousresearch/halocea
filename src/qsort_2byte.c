/* qsort_2byte 0x8381A508 — in-place sort of an array of 2-byte (__int16) elements using an iterative quicksort
 * with an explicit pending-partition stack and a selection-sort fallback for partitions of <= 8 elements. The
 * comparator returns nonzero when its first argument should sort before its second. Partitions are pushed
 * smaller-first so the explicit stack never exceeds ~log2(num) depth. Sibling of qsort_4byte (identical
 * structure, __int16 stride and a byte pivot offset of span & ~1).
 *
 * DEVIATION: the decompiler modeled the two parallel partition-bound stacks as single BYREF ints (v19/v20);
 * the surrounding stack frame shows each is really a 32-entry int array, declared as such here. */

void qsort_2byte(__int16 *base, unsigned int num, unsigned __int8 (*compare)(__int16, __int16))
{
    int stack_depth;
    __int16 *lo;
    __int16 *hi;
    int *hi_stack;
    int *lo_stack;
    unsigned int span;
    __int16 *max_elem;
    __int16 *i;
    __int16 tmp;
    unsigned int pivot_offset;
    __int16 *left;
    __int16 *right;
    __int16 swap_tmp;
    int hi_stack_storage[32];
    int lo_stack_storage[32];

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
            swap_tmp = *(__int16 *)((char *)lo + pivot_offset);
            *(__int16 *)((char *)lo + pivot_offset) = *lo;
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
                *++lo_stack = (int)left;
                ++stack_depth;
                *++hi_stack = (int)hi;
            }
            if ( lo + 1 >= right )
            {
pop_partition:
                --stack_depth;
                --lo_stack;
                --hi_stack;
                if ( stack_depth < 0 )
                    return;
                lo = (__int16 *)lo_stack[1];
                hi = (__int16 *)hi_stack[1];
            }
            else
            {
                hi = right - 1;
            }
        }

        if ( lo + 1 < right )
        {
            lo_stack[1] = (int)lo;
            ++stack_depth;
            hi_stack[1] = (int)(right - 1);
            ++lo_stack;
            ++hi_stack;
        }
        if ( left >= hi )
            goto pop_partition;
        lo = left;
    }
}
