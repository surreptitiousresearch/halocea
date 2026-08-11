/* ai_scripting_set_respawn_evaluate @0x83729C10 — HaloScript builtin (ai_set_respawn) evaluator.
 * Argument block: [0] = AI index (long), [4] = respawn (boolean). No return value. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void ai_scripting_set_respawn(int ai_index, uint8_t respawn);

void ai_scripting_set_respawn_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        ai_scripting_set_respawn(arguments[0], ((unsigned char *)arguments)[4]);
        hs_return(thread_index, 0);
    }
}
