/* numeric_countdown_timer_set_evaluate @0x83727AE8 — HaloScript builtin wrapper for numeric_countdown_timer_set;
 * (long, bool). */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void numeric_countdown_timer_set(int milliseconds, uint8_t auto_start);

void numeric_countdown_timer_set_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        /* HS argument block: packed wire, faithfully de-flagged */
        numeric_countdown_timer_set(arguments[0], ((unsigned char *)arguments)[4]);
        hs_return(thread_index, 0);
    }
}
