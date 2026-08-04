/* ai_scripting_migrate_and_speak_evaluate @0x8372A088 — HaloScript builtin (ai_migrate_and_speak)
 * evaluator. Argument block: [0] = source AI index (long), [4] = target AI index (long), [8] = speech type
 * (string). No return value. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void ai_scripting_migrate_and_speak(int source_index, int target_index, const char *speech_type);

void ai_scripting_migrate_and_speak_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        ai_scripting_migrate_and_speak(arguments[0], arguments[1], (const char *)arguments[2]);
        hs_return(thread_index, 0);
    }
}
