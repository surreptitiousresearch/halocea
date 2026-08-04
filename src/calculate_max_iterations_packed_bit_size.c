extern unsigned int bits_needed[];

unsigned int calculate_max_iterations_packed_bit_size(int max_iterations)
{
    if (max_iterations > 1)
        return bits_needed[max_iterations - 1];
    return 0;
}
