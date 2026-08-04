/* ai_scripting_force_active_by_unit_evaluate @0x8372A728 — HaloScript builtin (ai_force_active_by_unit)
 * evaluator. Argument block: [0] = unit index (long), [4] = active (boolean). No return value. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void ai_scripting_force_active_by_unit(int unit_index, char active);

void ai_scripting_force_active_by_unit_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        ai_scripting_force_active_by_unit(arguments[0], ((unsigned char *)arguments)[4]);
        hs_return(thread_index, 0);
    }
}
