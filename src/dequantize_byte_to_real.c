#include <stdint.h>
/* dequantize_byte_to_real @ 0x836FC0B8 — reconstruct a real in [min,max] from a byte quantized across
 * the 0..255 range. 255 maps exactly to max; every other value is a linear interpolation. Inverse of
 * quantize_real_to_byte.
 *
 * DEVIATION: min/max arrive in FPR-shadowed slots (f1/f2) and value in a GPR (r5), so the decompiler
 * invented phantom trailing int args; restored to the (float, float, byte) prototype matching the
 * quantize_real_to_byte_* siblings. */

float dequantize_byte_to_real(float min, float max, uint8_t value)
{
    if (value == 255)
        return max;
    return (float)value * 0.0039215689f * (max - min) + min;
}
