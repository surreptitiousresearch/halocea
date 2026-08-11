/* hs_players_evaluate @0x837271D8 — HaloScript builtin evaluator. No arguments; returns an object-list
 * reference of all player units. */
#include <stdint.h>

extern void hs_return(int thread_index, int value);
extern int hs_players(void);
void hs_players_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    hs_return(thread_index, hs_players());
}
