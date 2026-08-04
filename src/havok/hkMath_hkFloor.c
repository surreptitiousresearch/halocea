/* hkMath::hkFloor — branchless IEEE-754 floor. DEVIATION: reproduced verbatim
   from the decompiler's bit expression; the argument is a float (shown as
   double due to FP promotion) and the result is rebuilt from raw bits. The
   integer sub-expressions isolate the exponent, clear the fractional mantissa
   bits, and correct by -1.0 (0xBF800000) for negative non-integers. */
float hkMath_hkFloor(float r)
{
    int bits = *(int *)&r;
    int exponent = (unsigned char)(bits >> 23) - 127;
    int mant_mask = (-8388608 >> ((~((23 - exponent) >> 31) & 0x17)
                                  - (~((23 - exponent) >> 31) & (23 - exponent))))
                    | ((23 - exponent) >> 31);
    int nonzero = bits & ~(((bits & 0x7FFFFFFF) - 1) >> 31);
    int result = (((0x800000 >> exponent) & ~((nonzero & ~mant_mask) - 1) & (nonzero >> 31)) + nonzero)
                     & ~(exponent >> 31) & mant_mask
                 | (exponent >> 31) & (nonzero >> 31) & 0xBF800000;
    return *(float *)&result;
}
