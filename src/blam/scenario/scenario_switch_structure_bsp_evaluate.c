/* scenario_switch_structure_bsp_evaluate @0x8372BD78 — HaloScript builtin wrapper; (short). */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern uint8_t scenario_switch_structure_bsp(int16_t structure_bsp_index);

void scenario_switch_structure_bsp_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        scenario_switch_structure_bsp(*(int16_t *)arguments);
        hs_return(thread_index, 0);
    }
}
