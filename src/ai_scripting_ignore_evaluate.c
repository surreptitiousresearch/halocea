/* ai_scripting_ignore_evaluate @0x8372A350 — HaloScript builtin (ai_ignore) evaluator.
 * Argument block: [0] = object list index (long), [4] = ignore (boolean). No return value. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void ai_scripting_ignore(int object_list_index, uint8_t ignore);

void ai_scripting_ignore_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        ai_scripting_ignore(arguments[0], ((unsigned char *)arguments)[4]);
        hs_return(thread_index, 0);
    }
}
