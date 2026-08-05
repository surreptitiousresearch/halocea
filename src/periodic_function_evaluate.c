/* periodic_function_evaluate @ 0x837B6E40 — sample one of the engine's precomputed periodic waveform
 * tables (sine, triangle, sawtooth, etc.) at a given phase. Function type 0 is the constant 1.0. Each
 * table is 1024 bytes (one period); the phase is scaled to table space, the integer part indexes the
 * table and the fractional part linearly interpolates to the next entry (wrapping at 1024). Function
 * types whose bit falls in mask 0xC0 are "circular" (their value wraps like a hue): when the two
 * sampled entries straddle the 0/1 seam the upper one is lifted by 1 before the lerp and the result is
 * folded back into [0,1]. Returns 0 if the tables have not been built yet.
 *
 * Reconstructed against the disassembly; the decompiler's 64-bit register pairs and long-double ABI
 * artifacts (HIDWORD/LODWORD of v9/v10, the 1.0 high-word stores) are noise. 0.0039215689 == 1/255. */

#include <math.h>
#include <stdint.h>
#include "headers/periodic_function.h"
#include "headers/blam_data_globals.h"

extern unsigned char periodic_functions_built_flag;                    /* periodic/transition tables built flag */

float periodic_function_evaluate(int16_t function_type, float time)
{
    if ( !function_type )
        return 1.0f;
    if ( !periodic_functions_built_flag )
        return 0.0f;

    float scaled = time * 25.60000038146973f;
    float fraction = (float)fmod(scaled, 1.0);          /* interpolation weight */
    int index = (int)(scaled - fraction) & 0x3FF;

    const uint8_t *table = periodic_function_tables[function_type];
    float low  = table[index] * 0.0039215689f;
    float high = table[(index + 1) & 0x3FF] * 0.0039215689f;

    float result;
    if ( function_type == _periodic_function_slide
      || function_type == _periodic_function_slide_with_random_period )  /* circular function: wrap across the seam */
    {
        if ( low > 0.75f && high < 0.25f )
            high = high + 1.0f;
        result = (1.0f - fraction) * low + fraction * high;
        if ( result > 1.0f )
            result = result - 1.0f;
    }
    else
    {
        result = (1.0f - fraction) * low + fraction * high;
    }

    return result;
}
