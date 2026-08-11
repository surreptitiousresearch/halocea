/* ai_scripting_teleport_starting_location_if_unsupported_evaluate @0x8372A438 — HaloScript builtin
 * (ai_teleport_to_starting_location_if_unsupported) evaluator. Argument block: [0] = AI index (long).
 * No return value. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void ai_scripting_teleport_starting_location_if_unsupported(int ai_index);

void ai_scripting_teleport_starting_location_if_unsupported_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        ai_scripting_teleport_starting_location_if_unsupported(arguments[0]);
        hs_return(thread_index, 0);
    }
}
