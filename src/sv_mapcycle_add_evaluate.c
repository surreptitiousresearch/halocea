/* sv_mapcycle_add_evaluate @0x8372E9A8 — HaloScript builtin wrapper for the server command sv_mapcycle_add. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void sv_mapcycle_add(const char *map, const char *variant);

void sv_mapcycle_add_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        sv_mapcycle_add((const char *)arguments[0], (const char *)arguments[1]);
        hs_return(thread_index, 0);
    }
}
