/* sv_mapcycle_save_evaluate @0x8372E900 — HaloScript builtin wrapper for the server command sv_mapcycle_save.
 *
 * DEVIATION: sv_mapcycle_save is a void no-argument console builtin; the decompiler renders it as taking the evaluator's
 * (function_index, thread_index, initialize) triple, so it is called that way here. */

#include <stdint.h>

extern void hs_return(int thread_index, int value);
extern void sv_mapcycle_save(void);

void sv_mapcycle_save_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    sv_mapcycle_save();
    hs_return(thread_index, 0);
}
