/* ai_scripting_going_to_vehicle_evaluate @0x8372AFB0 — HaloScript builtin (ai_going_to_vehicle) evaluator.
 * Argument block: [0] = vehicle unit index (long). Returns the count in the result's high word. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern int16_t ai_scripting_going_to_vehicle(int unit_index);

void ai_scripting_going_to_vehicle_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        int result = 0;   /* LOWORD = 0 */
        *(int16_t *)&result = ai_scripting_going_to_vehicle(arguments[0]);
        hs_return(thread_index, result);
    }
}
