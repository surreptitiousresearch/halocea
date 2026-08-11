/* hs_object_set_permutation_evaluate @0x83727640 — HaloScript builtin evaluator. Argument [0] = object index
 * (long), [4] = region name (string), [8] = permutation name (string). No return value. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void hs_object_set_permutation(int object_index, const char *region_name, char *permutation_name);
void hs_object_set_permutation_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        hs_object_set_permutation(arguments[0], (const char *)arguments[1], (char *)arguments[2]);
        hs_return(thread_index, 0);
    }
}
