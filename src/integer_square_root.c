unsigned int integer_square_root(unsigned int k)
{
    unsigned int result = 0;
    unsigned int bit = 0x40000000;

    while (bit)
    {
        unsigned int candidate = bit + result;
        if (candidate <= k)
        {
            k -= candidate;
            result = candidate + bit;
        }
        bit >>= 2;
        result >>= 1;
    }

    return (k <= result) ? result : result + 1;
}
