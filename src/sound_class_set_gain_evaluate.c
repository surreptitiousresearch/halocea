/* sound_class_set_gain_evaluate @0x8372CA38 — HaloScript builtin wrapper for sound_class_set_gain.
 *
 * DEVIATION: the decompiler lost the third argument (an uninitialized register); the disassembly loads it
 * from a halfword at argument offset 8, reconstructed as *((__int16 *)arguments + 4). */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void sound_class_set_gain(const char *substring, float gain, int16_t ticks);

void sound_class_set_gain_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        /* HS argument block is packed wire (mixed-width slots): keep faithful indexed reads. */
        sound_class_set_gain((const char *)arguments[0], ((float *)arguments)[1], ((int16_t *)arguments)[4]);
        hs_return(thread_index, 0);
    }
}
