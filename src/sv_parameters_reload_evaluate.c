/* sv_parameters_reload_evaluate @0x8372E278 — HaloScript builtin wrapper for the server command sv_parameters_reload.
 *
 * DEVIATION: sv_parameters_reload is a void no-argument console builtin; the decompiler renders it as taking the evaluator's
 * (function_index, thread_index, initialize) triple, so it is called that way here. */

#include <stdint.h>

extern void hs_return(int thread_index, int value);
extern void sv_parameters_reload(void);

void sv_parameters_reload_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    sv_parameters_reload();
    hs_return(thread_index, 0);
}
