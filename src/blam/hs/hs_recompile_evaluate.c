#include <stdint.h>
#include "headers/blam_data_globals.h"
/* hs_recompile_evaluate @0x8372FF18 — HaloScript builtin evaluator. Sets the global recompile flag so the
 * script system rebuilds on the next opportunity; returns void. */

extern void hs_return(int thread_index, int value);

void hs_recompile_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    recompile = 1;
    hs_return(thread_index, 0);
}
