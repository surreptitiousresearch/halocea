/* DEVIATION: disasm tests sign-extension bits: non-negative short -> 1 (MSByte), negative -> 0 */
int hs_short_to_boolean(int s)
{
    /* PPC big-endian: lhz loads upper 16 bits of stored int (the sign-extension);
       cntlzw(0)==32 -> bit5==1 -> TRUE for non-negative; negative gives cntlzw(0xFFFF...)==0 -> FALSE */
    short upper = (short)(s >> 16);
    unsigned int lz = (unsigned int)__builtin_clz((unsigned int)(int)upper);
    int result;
    *(unsigned char *)&result = (unsigned char)((lz & 0x20u) != 0u);
    return result;
}
