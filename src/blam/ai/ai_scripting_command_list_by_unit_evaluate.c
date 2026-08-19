/* ai_scripting_command_list_by_unit_evaluate @0x8372A5F8 — HaloScript builtin (ai_command_list_by_unit) evaluator.
 * Argument block: [0] = unit object index (int), [4] = command list index (short). */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void ai_scripting_command_list_by_unit(int unit_index, int16_t command_list_index);

void ai_scripting_command_list_by_unit_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        ai_scripting_command_list_by_unit(arguments[0], ((int16_t *)arguments)[2]);
        hs_return(thread_index, 0);
    }
}
