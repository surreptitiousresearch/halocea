/* numeric_countdown_timer_restart_evaluate @0x83727BC8 — HaloScript builtin wrapper.
 *
 * DEVIATION: numeric_countdown_timer_restart is a void no-argument builtin; the decompiler renders it as taking the evaluator
 * (function_index, thread_index, initialize) triple, so it is called that way here. */

#include <stdint.h>

extern void hs_return(int thread_index, int value);
extern void numeric_countdown_timer_restart(void);

void numeric_countdown_timer_restart_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    numeric_countdown_timer_restart();
    hs_return(thread_index, 0);
}
