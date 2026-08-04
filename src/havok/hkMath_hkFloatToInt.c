/* hkMath::hkFloatToInt — branchless truncation of a float to int. DEVIATION:
   reproduced verbatim from the decompiler's bit expression (float arg shown as
   double via FP promotion). Isolates the exponent, extracts the significand,
   masks off fractional bits for small magnitudes, restores sign, and shifts
   into integer position. */
int hkMath_hkFloatToInt(float r)
{
    int bits = *(int *)&r;
    int exponent = (unsigned char)(bits >> 23) - 127;
    int small = ((unsigned char)(bits >> 23) - 151) >> 31;
    char shift = (23 - exponent) & small;
    int mantissa = (bits & ~(((bits & 0x7FFFFFFF) - 1) >> 31) & 0x7FFFFF | 0x800000)
                   & ((-8388608 >> ((small & 0x17) - shift)) | ~small);
    return ((((2 * mantissa - 1) | ((bits & ~(((bits & 0x7FFFFFFF) - 1) >> 31)) >> 31)) - mantissa + 1)
            & ~(exponent >> 31))
           >> shift << ((~(char)small) & ((bits >> 23) + 106));
}
