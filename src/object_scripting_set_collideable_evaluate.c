/* object_scripting_set_collideable_evaluate @0x83727EC8 — HaloScript builtin evaluator. Argument [0] = object index (long), [4] = boolean.
 * No return value. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void object_scripting_set_collideable(int object_index, uint8_t collideable);
void object_scripting_set_collideable_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments ) { object_scripting_set_collideable(arguments[0], ((unsigned char *)arguments)[4]); hs_return(thread_index, 0); }
}
