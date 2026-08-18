/* numeric_countdown_timer_get_evaluate @0x83727B38 — HaloScript builtin wrapper for numeric_countdown_timer_get;
 * (short) returning a short. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern int16_t numeric_countdown_timer_get(int16_t digit_index);

void numeric_countdown_timer_get_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    /* DEVIATION: `stw r11, 0x50(r1)` @0x83727B50 zeroes the WHOLE word — the halfword */
    /* form left slot bytes 2-3 uninitialised; `sth 0x50(r1)` @0x83727B68 puts the short at slot halfword 0. */
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        /* DEVIATION: endian-portable respelling of the BE high-halfword store (was an *(narrow*)&result pun; hs_cast extracts (int16_t)(value >> 16)) 2026-08-18 */
        int result = (int)((uint16_t)(numeric_countdown_timer_get(*(int16_t *)arguments))) << 16;
        hs_return(thread_index, result);
    }
}
