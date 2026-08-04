/* ai_scripting_conversation_evaluate @0x8372B250 — HaloScript builtin (ai_conversation) evaluator.
 * Argument block: [0] = conversation definition index (short). Returns the boolean "began" in the result's
 * high byte. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern uint8_t ai_scripting_conversation(int16_t conversation_index);

void ai_scripting_conversation_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    __int16 *arguments = (__int16 *)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        int result = 0;
        *((unsigned char *)&result + 3) = ai_scripting_conversation(arguments[0]);
        hs_return(thread_index, result);
    }
}
