/* shortsort_4byte @0x8381A480 — CRT qsort helper: selection sort of the 4-byte-element range [lo,hi]
 * (inclusive), used for small partitions during the sort's recursion. Identical algorithm to
 * shortsort_2byte.c at 4-byte element granularity. */
#include <stdint.h>

void shortsort_4byte(char *lo, char *hi, uint8_t (__fastcall *compare)(int, int))
{
    char *end = hi;

    if ( hi > lo )
    {
        do
        {
            int *record = (int *)lo;
            int *scan = (int *)(lo + 4);

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

            int temp = *record;
            *record = *(int *)end;
            *(int *)end = temp;
            end -= 4;
        }
        while ( end > lo );
    }
}
