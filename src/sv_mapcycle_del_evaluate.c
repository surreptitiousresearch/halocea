/* sv_mapcycle_del_evaluate @0x8372E9F8 — HaloScript builtin wrapper for the server command sv_mapcycle_del. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void sv_mapcycle_del(int index);

void sv_mapcycle_del_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        sv_mapcycle_del(*arguments);
        hs_return(thread_index, 0);
    }
}
