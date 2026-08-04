/* ai_scripting_living_fraction @0x8376FEE8 — return the fraction of the addressed AI still alive
 * (current count / original count), or 0 if nothing was ever spawned.
 *
 * DEVIATION: the decompiler reported "local variable allocation has failed" and produced garbled
 * 64-bit/endian shuffling for this body. Reconstructed from the disassembly (8376FEE8): count_internal
 * returns the current count and writes the original count through original_count_reference; the result is
 * the float quotient current/original when original > 0, else the pre-initialized 0.0. */

#include <stdint.h>
#include "headers/ai_count_type.h"
extern int ai_scripting_count_internal(unsigned int ai_index, int16_t count_type, int *original_count_reference, float *strength_reference);

float ai_scripting_living_fraction(int ai_index)
{
    int original_count = 0;
    int current_count = ai_scripting_count_internal(ai_index, _ai_count_living, &original_count, nullptr);
    if ( original_count > 0 )
        return (float)current_count / (float)original_count;
    return 0.0f;
}
