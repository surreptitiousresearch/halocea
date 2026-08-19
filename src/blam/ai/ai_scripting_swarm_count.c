/* ai_scripting_swarm_count @0x8376FEB8 — return the number of living swarm actors addressed by an AI
 * index. */

#include <stdint.h>
#include "headers/ai_count_type.h"

extern int ai_scripting_count_internal(unsigned int ai_index, int16_t count_type, int *original_count_reference, float *strength_reference);

int ai_scripting_swarm_count(int ai_index)
{
    return ai_scripting_count_internal(ai_index, _ai_count_swarm, nullptr, nullptr);
}
