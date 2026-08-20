/* data_allocation_size @ 0x836FAC48
   Bytes needed for a data_array holding maximum_count elements of `size` bytes:
   the 56-byte header plus inline element storage. */

#include <stdint.h>

int data_allocation_size(int16_t maximum_count, int16_t size)
{
    return maximum_count * size + 56;
}
