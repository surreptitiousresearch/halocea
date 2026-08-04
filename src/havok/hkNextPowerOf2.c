/* Round up to the next power of two via bit smearing. DEVIATION: the decompiler
   emits the smears in 16,8,4,2,1 order fused into two nested expressions; the
   result is the standard (in-1) | >>1.. | >>16, +1. */
unsigned int hkNextPowerOf2(unsigned int in)
{
    unsigned int v = in - 1;
    v |= v >> 16;
    v |= v >> 8;
    v |= v >> 4;
    v |= v >> 2;
    v |= v >> 1;
    return v + 1;
}
