/* add_feature @0x837E0A50 — append `index` to an index list (count + indices[256]) unless it is already present
 * or the list is full (256). */
#include <stdint.h>

void add_feature(int *count, int *indices, int index)
{
    if ( *count > 0 )
    {
        for ( int16_t i = 0; ; )
        {
            if ( indices[i] == index )
                return;                 /* already recorded */
            i = (int16_t)(i + 1);
            if ( i >= *count )
                break;
        }
    }
    if ( *count < 256 )
    {
        indices[*count] = index;
        ++*count;
    }
}
