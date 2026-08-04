/* numeric_countdown_timer_get_evaluate @0x83727B38 — HaloScript builtin wrapper for numeric_countdown_timer_get;
 * (short) returning a short. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern int16_t numeric_countdown_timer_get(int16_t digit_index);

void numeric_countdown_timer_get_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int result;
    *((__int16 *)&result) = 0;
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        *((__int16 *)&result + 1) = numeric_countdown_timer_get(*(__int16 *)arguments);
        hs_return(thread_index, result);
    }
}
