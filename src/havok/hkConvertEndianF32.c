/* 32-bit float byte swap. DEVIATION: the decompiler models the argument as a
   double and swaps the low four bytes in place; it is simply a byte swap of
   the float's 32-bit representation. */
float hkConvertEndianF32(float n)
{
    unsigned int bits = *(unsigned int *)&n;
    unsigned int swapped = (bits >> 24) | ((bits >> 8) & 0xFF00)
                         | ((bits << 8) & 0xFF0000) | (bits << 24);
    return *(float *)&swapped;
}
