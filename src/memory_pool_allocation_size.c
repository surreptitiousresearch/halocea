/* memory_pool_allocation_size @ 0x8371BBA8 — payload size plus the 56-byte header */

int memory_pool_allocation_size(int size)
{
    return size + 56;
}
