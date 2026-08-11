/* sv_end_game_evaluate @0x8372E7C8 — HaloScript builtin wrapper for the server command sv_end_game.
 *
 * DEVIATION: sv_end_game is a void no-argument console builtin; the decompiler renders it as taking the evaluator's
 * (function_index, thread_index, initialize) triple, so it is called that way here. */

#include <stdint.h>

extern void hs_return(int thread_index, int value);
extern void sv_end_game(void);

void sv_end_game_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    sv_end_game();
    hs_return(thread_index, 0);
}
