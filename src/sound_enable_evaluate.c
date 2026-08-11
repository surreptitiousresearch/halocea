/* sound_enable_evaluate @0x8372CA88 — HaloScript builtin wrapper for sound_enable; takes a boolean. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void sound_enable(uint8_t enabled);

void sound_enable_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    unsigned char *arguments = (unsigned char *)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        sound_enable(*arguments);
        hs_return(thread_index, 0);
    }
}
