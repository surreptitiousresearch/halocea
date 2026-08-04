#include <math.h>
#include <stdint.h>
#include "headers/periodic_function.h"
#include "headers/math_constants.h"

extern void periodic_function_build_variable_period_x_table(float *x_table);
extern float real_seed_random(uint32_t *seed);
extern uint32_t *get_global_random_seed_address(void);

void periodic_function_build_table(int16_t function_type, uint8_t *integer_table)
{
    float variable_period_x[1024];
    float samples[1024];
    float maximum = -3.4028235e38f;
    float minimum =  3.4028235e38f;
    float range;
    int   i;

    periodic_function_build_variable_period_x_table(variable_period_x);

    for (i = 0; i < 1024; i = (int16_t)(i + 1))
    {
        float fi        = (float)i;
        float phase     = fi * 0.027343748f;                /* 0..28 across the table */
        float var_phase = variable_period_x[i] * 28.0f;
        float value     = 0.0f;

        switch (function_type)
        {
        case _periodic_function_one:
            value = 1.0f;
            break;
        case _periodic_function_zero:
            value = 0.0f;
            break;
        case _periodic_function_cosine:
            value = (float)cos(phase * TWO_PI);
            break;
        case _periodic_function_cosine_with_random_period:
            value = (float)cos(var_phase * TWO_PI);
            break;
        case _periodic_function_diagonal_wave:
        {
            float f = (float)fmod(phase, 1.0);
            if (f >= 0.5f)
                value = -(((f - 0.5f) * 2.0f) - 1.0f);
            else
                value = f * 2.0f;
            break;
        }
        case _periodic_function_diagonal_wave_with_random_period:
        {
            float f = (float)fmod(var_phase, 1.0);
            if (f >= 0.5f)
                value = -(((f - 0.5f) * 2.0f) - 1.0f);
            else
                value = f * 2.0f;
            break;
        }
        case _periodic_function_slide:
            value = (float)fmod(phase, 1.0);
            break;
        case _periodic_function_slide_with_random_period:
            value = (float)fmod(var_phase, 1.0);
            break;
        case _periodic_function_noise:
            value = real_seed_random(get_global_random_seed_address());
            break;
        case _periodic_function_jitter:
        case _periodic_function_wander:
        {
            /* BINARY-FAITHFUL NOTE: in the binary case 9 (jitter, 0x837B756C) computes
               its own six sin/cos calls, discards every result (each f1 is overwritten
               before use; the f17 clobber is repaired by a stack reload @ 0x837B75F8),
               then FALLS THROUGH into case 10 (wander, 0x837B75A0). Net value for both
               types is the wander formula below, so the cases are merged here; the six
               dead pure-function calls are not reproduced. See FINDINGS. */
            float c0 = (float)cos(phase * TWO_PI);
            float s1 = (float)sin(phase * PI);
            float c2 = (float)cos(phase * 0.89759791f);
            float c3 = (float)cos(phase * 25.132742f);
            float s4 = (float)sin(phase * 1.5707964f);
            float c5 = (float)cos(phase * 43.9823f);
            value = (((c5 * s4) + (c3 * c2)) * 0.5f) + (s1 * c0);
            break;
        }
        case _periodic_function_spark:
        {
            float f = (float)fmod(var_phase, 1.0);
            value = f * f;
            break;
        }
        default:
            /* BINARY-FAITHFUL NOTE: for out-of-range types the binary jumps straight
               to the min/max update (bgt cr6 @ 0x837B7474 -> 0x837B7624) with f0
               STALE — on the first iteration f0 is an uninitialized stack read
               (lfs f0, var_20E0 @ 0x837B73D8). The original source left `value`
               unassigned here; the `value = 0.0f` initializer above is a
               compile-safe stand-in. Callers only pass 0..11. See FINDINGS
               (same pattern as transition_function_build_table). */
            break;
        }

        if (value > maximum)
            maximum = value;
        if (value < minimum)
            minimum = value;
        samples[i] = value;
    }

    /* slide functions (types 6 and 7) are emitted without rescaling */
    if (function_type == _periodic_function_slide
        || function_type == _periodic_function_slide_with_random_period)
        range = 0.0f;
    else
        range = maximum - minimum;

    for (i = 0; i < 1024; i = (int16_t)(i + 1))
    {
        float normalized = samples[i];
        int   scaled;

        if (range != 0.0f)
            normalized = (samples[i] - minimum) / range;

        scaled = (int)(normalized * 255.0f);
        if (scaled < 0)
            scaled = 0;
        else if (scaled > 255)
            scaled = 255;
        integer_table[i] = (uint8_t)scaled;
    }
}
