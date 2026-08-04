#include <stdint.h>
int16_t floor_log2(unsigned int k)
{
    int result = 0;
    if ( k != 0 && k != 1 )
    {
        do
        {
            k >>= 1;
            ++result;
        } while ( k != 1 );
    }
    return (__int16)result;
}
