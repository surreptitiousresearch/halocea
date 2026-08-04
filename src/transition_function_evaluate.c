/* transition_function_evaluate @ 0x837B6FA0 — remap an input in [0,1] through one of the engine's
 * precomputed transition curve tables (linear, early, late, very early, ..., cosine, one, zero). The
 * input is clamped to [0,1]; function type 0 is the identity. Each table is 1024 bytes; the input is
 * scaled to table space, indexed, and linearly interpolated to the next entry, with the last cell
 * (1023) held flat. Returns 0 if the tables have not been built yet.
 *
 * Reconstructed against the disassembly; the decompiler's overlapped 64-bit register pairs
 * (v7/v8/v11 and the &v8-1 read) are long-double ABI noise. 0.0039215689 == 1/255. */

#include <math.h>
#include <stdint.h>
#include "headers/blam_data_globals.h"

extern unsigned char periodic_functions_built_flag;                       /* periodic/transition tables built flag */

float transition_function_evaluate(int16_t function_type, float value)
{
    if ( value < 0.0f )
        value = 0.0f;
    else if ( value > 1.0f )
        value = 1.0f;

    if ( !function_type )
        return value;
    if ( !periodic_functions_built_flag )
        return 0.0f;

    float scaled = value * 1023.0f;
    float fraction = (float)fmod(scaled, 1.0);            /* interpolation weight */
    __int16 index = (__int16)(int)(scaled - 0.5f);

    const unsigned __int8 *table = transition_function_tables[function_type];
    if ( index == 1023 )
        return table[1023] * 0.0039215689f;

    float low  = table[index] * 0.0039215689f;
    float high = table[index + 1] * 0.0039215689f;
    return low * (1.0f - fraction) + high * fraction;
}
