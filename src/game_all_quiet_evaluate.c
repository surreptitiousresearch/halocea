/* game_all_quiet_evaluate @0x8372C340 — HaloScript builtin evaluator. Takes no arguments; returns a boolean packed into
 * the high byte of the result word. */

#include <stdint.h>

extern void hs_return(uint16_t thread_index, int value);
extern uint8_t game_all_quiet(void);

void game_all_quiet_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int result = 0;
    *((unsigned char *)&result + 3) = game_all_quiet();
    hs_return(thread_index, result);
}
