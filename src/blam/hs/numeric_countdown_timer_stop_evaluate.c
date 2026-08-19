/* numeric_countdown_timer_stop_evaluate @0x83727B90 — HaloScript builtin wrapper.
 *
 * DEVIATION: numeric_countdown_timer_stop is a void no-argument builtin; the decompiler renders it as taking the evaluator
 * (function_index, thread_index, initialize) triple, so it is called that way here. */

#include <stdint.h>

extern void hs_return(int thread_index, int value);
extern void numeric_countdown_timer_stop(void);

void numeric_countdown_timer_stop_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    numeric_countdown_timer_stop();
    hs_return(thread_index, 0);
}
