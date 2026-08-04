/* display_scenario_help_evaluate @0x8372DEF0 — HaloScript builtin evaluator. Argument block:
 * [0] = help string index (short). No return value. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void display_scenario_help(int16_t string_index);

void display_scenario_help_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    __int16 *arguments = (__int16 *)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        display_scenario_help(*arguments);
        hs_return(thread_index, 0);
    }
}
