#include <stdint.h>
/* umemcmp @ 0x8376D458
   Byte-wise memory compare; returns signed difference of first differing byte, else 0. */

int umemcmp(uint8_t *buffer1, uint8_t *buffer2, unsigned int count)
{
    int result = 0;

    if ( count )
    {
        unsigned __int8 *end = buffer1 + count;
        do
        {
            int a = *buffer1;
            int b = *buffer2;
            result = a - b;
            if ( a != b )
                break;
            ++buffer1;
            ++buffer2;
        }
        while ( buffer1 != end );
    }
    return result;
}
