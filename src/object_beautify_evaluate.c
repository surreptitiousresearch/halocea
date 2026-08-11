/* object_beautify_evaluate @0x837280D8 — HaloScript builtin evaluator. Argument [0] = object index (long), [4] = boolean.
 * No return value. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void object_beautify(int object_index, uint8_t beautiful);
void object_beautify_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments ) { object_beautify(arguments[0], ((unsigned char *)arguments)[4]); hs_return(thread_index, 0); }
}
