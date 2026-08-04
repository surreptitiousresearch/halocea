/* ai_scripting_berserk_evaluate @0x8372ADC0 — HaloScript builtin (ai_berserk) evaluator.
 * Argument block: [0] = ai index (int), [4] = berserk (boolean byte). */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void ai_scripting_berserk(int ai_index, uint8_t berserk);

void ai_scripting_berserk_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        ai_scripting_berserk(arguments[0], ((unsigned char *)arguments)[4]);
        hs_return(thread_index, 0);
    }
}
