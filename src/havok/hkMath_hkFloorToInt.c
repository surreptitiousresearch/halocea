/* hkMath::hkFloorToInt — branchless floor-to-int. DEVIATION: reproduced
   verbatim from the decompiler's bit expression (float arg shown as double via
   FP promotion). As hkFloatToInt but the mantissa build biases negative
   non-integers downward before shifting into integer position. */
int hkMath_hkFloorToInt(float r)
{
    int bits = *(int *)&r;
    int exponent = (unsigned char)(bits >> 23) - 127;
    int small = ((unsigned char)(bits >> 23) - 151) >> 31;
    char shift = (23 - exponent) & small;
    int nonzero = bits & ~(((bits & 0x7FFFFFFF) - 1) >> 31);
    int mant_mask = (-8388608 >> ((small & 0x17) - shift)) | ~small;
    int mantissa = (((exponent >> 31) & (nonzero >> 31) | ~(exponent >> 31)) & 0x800000
                    | (((0x800000 >> exponent) & ~((nonzero & ~mant_mask) - 1) & (nonzero >> 31))
                       + (~(exponent >> 31) & nonzero)) & 0x7FFFFF)
                   & mant_mask;
    return (((2 * mantissa - 1) | (nonzero >> 31)) - mantissa + 1)
           >> shift << ((~(char)small) & ((bits >> 23) + 106));
}
