/* unit_start_user_animation_evaluate @0x83728640 — HaloScript builtin wrapper; packs a boolean result. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern uint8_t unit_start_user_animation(int unit_index, int animation_graph_index, const char *animation_name, uint8_t interpolate);

void unit_start_user_animation_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int result = 0;
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        /* DEVIATION: the decompiler packed the uint8 return via `*((unsigned char *)&result + 3)`.
         * Byte +3 of a 32-bit word is the LOW byte only on big-endian PPC; on x64 it is the HIGH
         * byte, so the literal spelling silently breaks the port. `result` starts at 0 and only
         * that byte is written, so a plain widening assignment is identical on PPC and correct
         * everywhere (same endian-unsafe class as the E4 round).
         * HS packed arg block: byte +12 is the boolean interpolate arg — slot[3]'s first byte,
         * matching the slot-byte-0 boolean convention of the other _evaluate shims. */
        result = unit_start_user_animation(
            arguments[0], arguments[1], (const char *)arguments[2], ((unsigned char *)arguments)[12]);
        hs_return(thread_index, result);
    }
}
