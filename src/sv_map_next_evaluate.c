/* sv_map_next_evaluate @0x8372E708 — HaloScript builtin wrapper for the server command sv_map_next.
 *
 * DEVIATION: sv_map_next is a void no-argument console builtin; the decompiler renders it as taking the evaluator's
 * (function_index, thread_index, initialize) triple, so it is called that way here. */

#include <stdint.h>

extern void hs_return(uint16_t thread_index, int value);
extern void sv_map_next(void);

void sv_map_next_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    sv_map_next();
    hs_return(thread_index, 0);
}
