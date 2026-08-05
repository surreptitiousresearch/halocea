/* cinematic_set_title_evaluate @0x8372C198 — HaloScript builtin (cinematic_set_title) evaluator.
 * Argument block: [0] = title index (short). No return value. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void cinematic_set_title(int16_t index);

void cinematic_set_title_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int16_t *arguments = (int16_t *)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        cinematic_set_title(*arguments);
        hs_return(thread_index, 0);
    }
}
