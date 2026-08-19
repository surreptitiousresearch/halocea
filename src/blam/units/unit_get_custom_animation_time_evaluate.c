/* unit_get_custom_animation_time_evaluate @0x837285A0 — wrapper; packs a short into the value slot's first halfword. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern int16_t unit_get_custom_animation_time(int unit_index);

void unit_get_custom_animation_time_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    /* DEVIATION: `li r11,0 / stw r11, 0x50(r1)` @0x837285B8 zeroes the WHOLE word, not a halfword. */
    /* DEVIATION: `sth r3, 0x50(r1)` @0x837285D0 repeats that displacement — the short is the slot's FIRST halfword. */
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        /* DEVIATION: endian-portable respelling of the BE high-halfword store (was an *(narrow*)&result pun; hs_cast extracts (int16_t)(value >> 16)) 2026-08-18 */
        int result = (int)((uint16_t)(unit_get_custom_animation_time(arguments[0]))) << 16;
        hs_return(thread_index, result);
    }
}
