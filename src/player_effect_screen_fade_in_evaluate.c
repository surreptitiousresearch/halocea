/* player_effect_screen_fade_in_evaluate @0x8372BF88 — HaloScript builtin wrapper for player_effect_screen_fade_in.
 *
 * DEVIATION: the decompiler rendered the trailing tick count as (__int16)pointer; the disassembly loads it
 * from a halfword at argument offset 0xC, reconstructed as *((__int16 *)arguments + 6). */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void player_effect_screen_fade_in(float red, float green, float blue, int16_t ticks);

void player_effect_screen_fade_in_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        player_effect_screen_fade_in(*(float *)arguments, ((float *)arguments)[1], ((float *)arguments)[2], ((int16_t *)arguments)[6]);
        hs_return(thread_index, 0);
    }
}
