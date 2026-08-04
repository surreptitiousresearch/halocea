/* shortsort_2byte @0x8381A3F8 — CRT qsort helper: selection sort of the 2-byte-element range [lo,hi]
 * (inclusive), used for small partitions during the sort's recursion. Each pass scans [lo+2,end] for the
 * element `compare` prefers over the running record (starting at lo), swaps it into `end`, then shrinks
 * `end` by one element. */

void shortsort_2byte(char *lo, char *hi, unsigned __int8 (__fastcall *compare)(__int16, __int16))
{
    char *end = hi;

    if ( hi > lo )
    {
        do
        {
            __int16 *record = (__int16 *)lo;
            __int16 *scan = (__int16 *)(lo + 2);

            if ( (char *)scan <= end )
            {
                do
                {
                    if ( compare(*scan, *record) )
                        record = scan;
                    ++scan;
                }
                while ( (char *)scan <= end );
            }

            __int16 temp = *record;
            *record = *(__int16 *)end;
            *(__int16 *)end = temp;
            end -= 2;
        }
        while ( end > lo );
    }
}
