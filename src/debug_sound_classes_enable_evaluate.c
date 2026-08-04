/* debug_sound_classes_enable_evaluate @0x8372C9C8 — HaloScript builtin evaluator. Argument block:
 * [0] = name substring (string pointer), [4] = boolean enable. No return value. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void debug_sound_classes_enable(const char *substring, uint8_t enabled);

void debug_sound_classes_enable_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        /* HS argument block: packed wire, faithfully de-flagged */
        debug_sound_classes_enable((const char *)arguments[0], ((unsigned char *)arguments)[4]);
        hs_return(thread_index, 0);
    }
}
