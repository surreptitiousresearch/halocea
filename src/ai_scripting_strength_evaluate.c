/* ai_scripting_strength_evaluate @0x8372B0B0 — HaloScript builtin (ai_strength) evaluator.
 * Argument block: [0] = AI index (long). Returns the strength fraction as a real (whole 32-bit result). */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern float ai_scripting_strength(int ai_index);

void ai_scripting_strength_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        int result;
        *(float *)&result = ai_scripting_strength(arguments[0]);
        hs_return(thread_index, result);
    }
}
