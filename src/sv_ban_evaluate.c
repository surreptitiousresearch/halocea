/* sv_ban_evaluate @0x8372E880 — HaloScript builtin wrapper for the server command sv_ban. */

#include <stdint.h>

/* extern corrected to match def (hs_macro_function_evaluate.c): returns int*. */
extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void sv_ban(const char *str);

void sv_ban_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        sv_ban((const char *)arguments[0]);
        hs_return(thread_index, 0);
    }
}
