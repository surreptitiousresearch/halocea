/* ai_scripting_living_count @0x8376FED8 — return the number of living actors addressed by an AI index. */

#include <stdint.h>
#include "headers/ai_count_type.h"
extern int ai_scripting_count_internal(unsigned int ai_index, int16_t count_type, int *original_count_reference, float *strength_reference);

int ai_scripting_living_count(int ai_index)
{
    return ai_scripting_count_internal(ai_index, _ai_count_living, nullptr, nullptr);
}
