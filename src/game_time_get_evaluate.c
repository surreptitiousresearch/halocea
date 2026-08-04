/* game_time_get_evaluate @0x8372B678 — HaloScript builtin evaluator. No arguments; returns the current game
 * time (in ticks) directly as the result. */

#include <stdint.h>

extern void hs_return(uint16_t thread_index, int value);
extern int game_time_get(void);

void game_time_get_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    hs_return(thread_index, game_time_get());
}
