/* hs_object_list_get_element_evaluate @0x83727690 — HaloScript builtin evaluator. Argument [0] = object list
 * (long), [4] = element index (short). Returns the object index at that position. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern int hs_object_list_get_element(int object_list_index, int16_t element_index);
void hs_object_list_get_element_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        hs_return(thread_index, hs_object_list_get_element(arguments[0], ((int16_t *)arguments)[2]));
    }
}
