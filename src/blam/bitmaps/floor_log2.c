/* floor_log2 @0x8379A3A8 */
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
    return (int16_t)result;
}
