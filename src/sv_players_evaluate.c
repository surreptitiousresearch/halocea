/* sv_players_evaluate @0x8372E800 — HaloScript builtin wrapper for the server command sv_players.
 *
 * DEVIATION: sv_players is a void no-argument console builtin; the decompiler renders it as taking the evaluator's
 * (function_index, thread_index, initialize) triple, so it is called that way here. */

#include <stdint.h>

extern void hs_return(uint16_t thread_index, int value);
extern void sv_players(void);

void sv_players_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    sv_players();
    hs_return(thread_index, 0);
}
