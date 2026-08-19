/* cinematic_set_title_delayed_evaluate @0x8372C1E0 — HaloScript builtin (cinematic_set_title_delayed)
 * evaluator. Argument block: [0] = title index (short), [4] = delay seconds (float). No return value. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void cinematic_set_title_delayed(int16_t index, float delay);

void cinematic_set_title_delayed_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        /* HS argument block: packed wire, faithfully de-flagged */
        cinematic_set_title_delayed(*(int16_t *)arguments, ((float *)arguments)[1]);
        hs_return(thread_index, 0);
    }
}
