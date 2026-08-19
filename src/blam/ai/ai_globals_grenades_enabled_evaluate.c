/* ai_globals_grenades_enabled_evaluate @0x83729798 — HaloScript builtin evaluator. Argument block:
 * [0] = boolean enable. No return value. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void ai_globals_grenades_enabled(uint8_t enable);

void ai_globals_grenades_enabled_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    unsigned char *arguments = (unsigned char *)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        ai_globals_grenades_enabled(*arguments);
        hs_return(thread_index, 0);
    }
}
