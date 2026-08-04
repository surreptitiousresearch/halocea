int memory_pool_block_compute_actual_size(int size)
{
    int result = size + 24;
    if (result & 3)
        return (result | 3) + 1;
    return result;
}
