/* ai_scripting_conversation_status_evaluate @0x8372B300 — HaloScript builtin (ai_conversation_status)
 * evaluator. Argument block: [0] = conversation definition index (short). Returns the status code packed in
 * the result's high word. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern int ai_scripting_conversation_status(__int16 conversation_index);

void ai_scripting_conversation_status_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    __int16 *arguments = (__int16 *)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        int result = 0;   /* LOWORD = 0 */
        *((__int16 *)&result + 1) = ai_scripting_conversation_status(arguments[0]);
        hs_return(thread_index, result);
    }
}
