/* ai_scripting_exit_vehicle_evaluate @0x8372A268 — HaloScript builtin (ai_exit_vehicle) evaluator.
 * Argument block: [0] = AI index (long). No return value. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void ai_scripting_exit_vehicle(int ai_index);

void ai_scripting_exit_vehicle_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        ai_scripting_exit_vehicle(arguments[0]);
        hs_return(thread_index, 0);
    }
}
