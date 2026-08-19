#include <stdint.h>
/* ceiling_log2 @ 0x8379A3E0
   Returns ceil(log2(k)): the smallest power-of-two exponent whose value is
   >= k. Returns 1 for k==0 and k==1 (matching the observed shift loop, which
   counts shifts of (k-1) down to 1 and adds one). */

int16_t ceiling_log2(unsigned int k)
{
    int16_t count = 0;

    if ( k != 0 )
    {
        unsigned int value = k - 1;
        if ( k != 2 )
        {
            do
            {
                value >>= 1;
                ++count;
            } while ( value != 1 );
        }
    }
    return (int16_t)(count + 1);
}
