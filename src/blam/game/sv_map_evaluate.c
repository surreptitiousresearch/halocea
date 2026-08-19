/* sv_map_evaluate @0x8372E778 — HaloScript builtin wrapper for the server command sv_map. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void sv_map(const char *mapname, const char *variantname);

void sv_map_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    /* int* arg block per the hs wrapper idiom (was const char** — C4047); args cast per-slot */
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        sv_map((const char *)arguments[0], (const char *)arguments[1]);
        hs_return(thread_index, 0);
    }
}
