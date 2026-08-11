/* ai_scripting_attach_free_evaluate @0x83729910 — HaloScript builtin (ai_attach_free) evaluator.
 * Argument block: [0] = unit object index (int), [4] = actor-variant tag index (int). */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void ai_scripting_attach_free(int unit_index, int actor_variant_tag_index);

void ai_scripting_attach_free_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        ai_scripting_attach_free(arguments[0], arguments[1]);
        hs_return(thread_index, 0);
    }
}
