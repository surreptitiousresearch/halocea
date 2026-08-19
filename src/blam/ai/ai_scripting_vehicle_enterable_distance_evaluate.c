/* ai_scripting_vehicle_enterable_distance_evaluate @0x8372A8F0 — HaloScript builtin
 * (ai_vehicle_enterable_distance) evaluator. Argument block: [0] = vehicle unit index (long),
 * [4] = distance (real). No return value. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void ai_scripting_vehicle_enterable_distance(int unit_index, float distance);

void ai_scripting_vehicle_enterable_distance_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        ai_scripting_vehicle_enterable_distance(arguments[0], ((float *)arguments)[1]);
        hs_return(thread_index, 0);
    }
}
