/* object_list_count_evaluate @0x837276E0 — HaloScript builtin evaluator. Argument [0] = object list (long).
 * Returns the count as a short packed into the high word of the result (low word zero). */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern uint16_t object_list_count(int object_list_index);
void object_list_count_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        int result = 0;
        *((int16_t *)&result + 1) = object_list_count(arguments[0]);
        hs_return(thread_index, result);
    }
}
