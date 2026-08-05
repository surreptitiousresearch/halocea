#include <stdint.h>
/* quantize_real_to_byte_lower_bound @0x83? — quantize a real in [min,max] to a byte such that the byte's
 * dequantized value is the greatest one not exceeding the input (a lower-bound / floor quantization). The
 * value is first scaled linearly to 0..255, then the candidate byte is decremented until its reconstructed
 * value is <= the input, guarding against rounding that would push the reconstruction above the input.
 *
 * DEVIATION: the three float args are passed in FPR-shadowed GPR slots, so the decompiler invented several
 * phantom trailing int args; restored to the database's (float, float, float) prototype. */

uint8_t quantize_real_to_byte_lower_bound(float min, float max, float value)
{
    uint8_t result = (uint8_t)(int)((value - min) / (max - min) * 255.0f);
    int candidate = result;
    if (result != 0)
    {
        do
        {
            float reconstructed;
            if (candidate == 255)
                reconstructed = max;
            else
                reconstructed = (float)candidate * 0.0039215689f * (max - min) + min;
            if (value >= reconstructed)
                break;
            result = (uint8_t)(candidate - 1);
            candidate = result;
        }
        while (result);
    }
    return result;
}
