/* ai_scripting_braindead_evaluate @0x8372A2B0 — HaloScript builtin (ai_braindead) evaluator.
 * Argument block: [0] = ai index (int), [4] = braindead (boolean byte). */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void ai_scripting_braindead(int ai_index, uint8_t braindead);

void ai_scripting_braindead_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        ai_scripting_braindead(arguments[0], ((unsigned char *)arguments)[4]);
        hs_return(thread_index, 0);
    }
}
