/* lruv_allocation_size @0x8371C018 — total allocation size for an lruv_cache's block table (28-byte block
 * entries) plus the 68-byte lruv_cache header. */

#include <stdint.h>

extern int data_allocation_size(int16_t maximum_count, int16_t size);

int lruv_allocation_size(int16_t maximum_block_count)
{
    return data_allocation_size(maximum_block_count, 28) + 68;
}
