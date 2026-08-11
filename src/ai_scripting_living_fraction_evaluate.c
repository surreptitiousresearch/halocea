/* ai_scripting_living_fraction_evaluate @0x8372B060 — HaloScript builtin (ai_living_fraction) evaluator.
 * Argument block: [0] = AI index (long). Returns the living fraction as a real (whole 32-bit result). */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern float ai_scripting_living_fraction(int ai_index);

void ai_scripting_living_fraction_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        int result;
        *(float *)&result = ai_scripting_living_fraction(arguments[0]);  /* float bits packed into the 32-bit hs value */
        hs_return(thread_index, result);
    }
}
