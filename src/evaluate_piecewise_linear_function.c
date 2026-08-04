/* evaluate_piecewise_linear_function @0x836DE538 — samples a piecewise-linear curve. |x| in [0,1] maps
 * across the `count` evenly-spaced samples in function[]; the result is linearly interpolated between
 * the two bracketing samples and given the sign of x.
 *
 * DEVIATION: the decompiler reported a local-allocation failure and emitted a phantom __int64 param;
 * reconstructed from disassembly. The real signature (count, function, x) matches the DB prototype. */

#include <stdint.h>

extern float __fabs(float x);

float evaluate_piecewise_linear_function(int16_t count, const float *function, float x)
{
    int   negative = (x < 0.0f);
    int   last = count - 1;
    float abs_x = __fabs(x);

    /* position along the table, clamped to [0, count-1] */
    float position = 0.0f;
    if (abs_x * (double)last >= 0.0)   /* guard: count >= 1 */
    {
        position = (float)(abs_x * (double)last);
        float ceiling = (float)count - 1.0f;
        if (position > ceiling)
            position = ceiling;
    }

    int i0 = (int16_t)(int)position;
    if (i0 < 0)
        i0 = 0;
    else if (i0 > last)
        i0 = last;

    int i1 = i0 + 1;
    if (i1 > last)
        i1 = last;

    float frac = position - (float)i0;
    float value = function[i0] + (function[i1] - function[i0]) * frac;

    if (negative)
        value = -value;
    return value;
}
