#include <math.h>
#include <stdint.h>
#include "headers/transition_function.h"
#include "headers/math_constants.h"

void transition_function_build_table(int16_t function_type, uint8_t *integer_table)
{
    int i;

    for (i = 0; i < 1024; i = (int16_t)(i + 1))
    {
        float x     = (float)i * 0.00097751711f;   /* 0..1 across the table */
        float value = x;                           /* type 0: linear */
        int   scaled;

        switch ((uint16_t)function_type)
        {
        case _transition_function_linear:
            value = x;
            break;
        case _transition_function_early:
            value = (float)pow(x, 0.5);
            break;
        case _transition_function_very_early:
            value = (float)pow(x, 0.25);
            break;
        case _transition_function_late:
            value = (float)pow(x, 2.0);
            break;
        case _transition_function_very_late:
            value = (float)pow(x, 4.0);
            break;
        case _transition_function_cosine:
            /* attest: single-precision throughout — 0x837B7320 fmsubs (x*pi - pi/2 in
               single), 0x837B7328 frsp(sin), then fadds/fmuls; was double add/mul. */
            value = ((float)sin(x * PI - 1.5707964f) + 1.0f) * 0.5f;
            break;
        default:
            /* BINARY-FAITHFUL NOTE: for out-of-range types the binary branches straight
               to the *255 scale (0x837B72B8 bgt cr6) with f0 STALE (uninitialized on the
               first iteration) — the original source left `value` unassigned here. The
               `value = x` initializer above is a compile-safe stand-in; callers only
               pass 0..5. See FINDINGS. */
            break;
        }

        scaled = (int)(value * 255.0f);
        if (scaled < 0)
            scaled = 0;
        else if (scaled > 255)
            scaled = 255;
        integer_table[i] = (uint8_t)scaled;
    }
}
