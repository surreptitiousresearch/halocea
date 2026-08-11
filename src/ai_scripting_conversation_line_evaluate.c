/* ai_scripting_conversation_line_evaluate @0x8372B2A8 — HaloScript builtin (ai_conversation_line) evaluator.
 * Argument block: [0] = conversation definition index (short). Returns the current line index packed in the
 * result's high word. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern int ai_scripting_conversation_line(int16_t conversation_index);

void ai_scripting_conversation_line_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int16_t *arguments = (int16_t *)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        int result = 0;   /* LOWORD = 0 */
        *(int16_t *)&result = ai_scripting_conversation_line(arguments[0]);
        hs_return(thread_index, result);
    }
}
