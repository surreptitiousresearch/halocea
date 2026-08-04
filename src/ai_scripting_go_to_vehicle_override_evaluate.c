/* ai_scripting_go_to_vehicle_override_evaluate @0x8372A218 — HaloScript builtin (ai_go_to_vehicle_override)
 * evaluator. Argument block: [0] = AI index (long), [4] = vehicle unit index (long), [8] = seat name
 * (string). No return value. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void ai_scripting_go_to_vehicle_override(int ai_index, int unit_index, const char *seat_substring_name);

void ai_scripting_go_to_vehicle_override_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        ai_scripting_go_to_vehicle_override(arguments[0], arguments[1], (const char *)arguments[2]);
        hs_return(thread_index, 0);
    }
}
