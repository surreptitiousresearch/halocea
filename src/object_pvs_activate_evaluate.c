/* object_pvs_activate_evaluate @0x83728280 — HaloScript builtin evaluator. Argument [0] = long. No return value. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void object_pvs_activate(int object_index);
void object_pvs_activate_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments ) { object_pvs_activate(arguments[0]); hs_return(thread_index, 0); }
}
