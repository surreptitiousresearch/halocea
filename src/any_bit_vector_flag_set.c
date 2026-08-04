int any_bit_vector_flag_set(unsigned int *bit_vector, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (bit_vector[i])
            return 1;
    }
    return 0;
}
