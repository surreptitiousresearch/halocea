/* ai_scripting_strength @0x8376FF58 — return the current strength fraction of the addressed AI (the
 * strength_reference output of ai_scripting_count_internal). */

#include <stdint.h>
#include "headers/ai_count_type.h"

extern int ai_scripting_count_internal(unsigned int ai_index, int16_t count_type, int *original_count_reference, float *strength_reference);

float ai_scripting_strength(int ai_index)
{
    float strength = 0.0f;
    ai_scripting_count_internal(ai_index, _ai_count_living, nullptr, &strength);
    return strength;
}
