/* sv_kick_evaluate @0x8372E838 — HaloScript builtin wrapper for the server command sv_kick. */

#include <stdint.h>

/* def (src/hs_macro_function_evaluate.c) returns `int *` (the evaluated argument block); matched here. */
extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void sv_kick(const char *str);

void sv_kick_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        sv_kick((const char *)arguments[0]);
        hs_return(thread_index, 0);
    }
}
