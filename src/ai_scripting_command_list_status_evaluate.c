/* ai_scripting_command_list_status_evaluate @0x8372AF58 — HaloScript builtin (ai_command_list_status) evaluator.
 * Argument block: [0] = object-list index (int). Returns the short status code in the result's high 16 bits
 * (the result's low 16 bits are 0), matching the engine's HIWORD packing of a short HS return value. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern int16_t ai_scripting_command_list_status(int object_list_index);

void ai_scripting_command_list_status_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        int result = ai_scripting_command_list_status(arguments[0]) << 16;
        hs_return(thread_index, result);
    }
}
