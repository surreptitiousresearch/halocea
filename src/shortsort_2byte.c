/* shortsort_2byte @0x8381A3F8 — CRT qsort helper: selection sort of the 2-byte-element range [lo,hi]
 * (inclusive), used for small partitions during the sort's recursion. Each pass scans [lo+2,end] for the
 * element `compare` prefers over the running record (starting at lo), swaps it into `end`, then shrinks
 * `end` by one element. */
#include <stdint.h>

void shortsort_2byte(char *lo, char *hi, uint8_t (__fastcall *compare)(int16_t, int16_t))
{
    char *end = hi;

    if ( hi > lo )
    {
        do
        {
            int16_t *record = (int16_t *)lo;
            int16_t *scan = (int16_t *)(lo + 2);

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

            int16_t temp = *record;
            *record = *(int16_t *)end;
            *(int16_t *)end = temp;
            end -= 2;
        }
        while ( end > lo );
    }
}
