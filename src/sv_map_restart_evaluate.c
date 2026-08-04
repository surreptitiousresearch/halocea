/* sv_map_restart_evaluate @0x8372E740 — HaloScript builtin wrapper for the server command sv_map_restart.
 *
 * DEVIATION: sv_map_restart is a void no-argument console builtin; the decompiler renders it as taking the evaluator's
 * (function_index, thread_index, initialize) triple, so it is called that way here. */

#include <stdint.h>

extern void hs_return(uint16_t thread_index, int value);
extern void sv_map_restart(void);

void sv_map_restart_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    sv_map_restart();
    hs_return(thread_index, 0);
}
