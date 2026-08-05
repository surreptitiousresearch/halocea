/* ai_scripting_conversation_stop_evaluate @0x8372ACE0 — HaloScript builtin (ai_conversation_stop) evaluator.
 * Argument block: [0] = conversation definition index (short). No return value. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void ai_scripting_conversation_stop(int16_t conversation_index);

void ai_scripting_conversation_stop_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int16_t *arguments = (int16_t *)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        ai_scripting_conversation_stop(arguments[0]);
        hs_return(thread_index, 0);
    }
}
