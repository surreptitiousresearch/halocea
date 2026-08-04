/* count_bits_32 @0x8382AF00 — population count of a 32-bit value (tests each bit via a rotating mask,
 * matching the PPC rotlwi lowering the compiler chose). */

int count_bits_32(int value)
{
    int result = 0;
    unsigned int mask = 1;
    for ( int i = 32; i; --i )
    {
        if ( mask & value )
            ++result;
        mask = (mask << 1) | (mask >> 31);
    }
    return result;
}
