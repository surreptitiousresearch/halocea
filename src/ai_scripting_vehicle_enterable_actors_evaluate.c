/* ai_scripting_vehicle_enterable_actors_evaluate @0x8372A9E0 — HaloScript builtin
 * (ai_vehicle_enterable_actors) evaluator. Argument block: [0] = vehicle unit index (long),
 * [4] = AI index (long). No return value. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void ai_scripting_vehicle_enterable_actors(int unit_index, int ai_index);

void ai_scripting_vehicle_enterable_actors_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        ai_scripting_vehicle_enterable_actors(arguments[0], arguments[1]);
        hs_return(thread_index, 0);
    }
}
