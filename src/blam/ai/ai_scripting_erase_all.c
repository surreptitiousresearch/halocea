/* ai_scripting_erase_all @0x8376F8B0 — erase every actor in the simulation. Called directly with the
 * evaluator's parameters, which it ignores. */

#include <stdint.h>

extern void ai_erase(int encounter_index, int platoon_index, int squad_index, uint8_t immediate);

void ai_scripting_erase_all(int16_t function_index, int thread_index, uint8_t initialize)
{
    (void)function_index;
    (void)thread_index;
    (void)initialize;
    ai_erase(-1, -1, -1, 0);
}
