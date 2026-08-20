#include <stdint.h>
/* quantize_real_to_byte_upper_bound @0x837008D0 — quantize a real in [min,max] to a byte such that the
 * byte's dequantized value is the smallest one not less than the input (an upper-bound / ceiling
 * quantization) — the mirror image of quantize_real_to_byte_lower_bound.c. The value is first scaled
 * linearly to 0..255, then the candidate byte is incremented until its reconstructed value is >= the input.
 *
 * DEVIATION: same class as quantize_real_to_byte_lower_bound.c — the three float args are passed in
 * FPR-shadowed GPR slots, so the decompiler invented several phantom trailing int args (confirmed dead via
 * disasm_range(0x837008D0,0x83700954): only f1/f2/f3 are ever read); restored to the database's
 * (float, float, float) prototype. */

uint8_t quantize_real_to_byte_upper_bound(float min, float max, float value)
{
    uint8_t result = (uint8_t)(int)((value - min) / (max - min) * 255.0f);
    int candidate = result;

    while ( result != 255 )
    {
        float reconstructed;

        if ( candidate == 255 )
            reconstructed = max;
        else
            reconstructed = (float)candidate * 0.0039215689f * (max - min) + min;

        if ( value <= reconstructed )
            break;

        result = (uint8_t)(candidate + 1);
        candidate = result;
    }

    return result;
}
