/* players_unzoom_all_evaluate @0x8372B740 — HaloScript builtin wrapper for players_unzoom_all.
 *
 * DEVIATION: players_unzoom_all is a void no-argument builtin; the decompiler renders it as taking the evaluator's
 * (function_index, thread_index, initialize) triple, so it is called that way here. */

#include <stdint.h>

extern void hs_return(uint16_t thread_index, int value);
extern void players_unzoom_all(void);

void players_unzoom_all_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    players_unzoom_all();
    hs_return(thread_index, 0);
}
