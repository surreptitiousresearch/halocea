/* scripting_set_magic_base_seat_evaluate @0x83728C70 — HaloScript builtin wrapper for scripting_set_magic_base_seat; one string. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void scripting_set_magic_base_seat(const char *string);

void scripting_set_magic_base_seat_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        scripting_set_magic_base_seat((const char *)arguments[0]);
        hs_return(thread_index, 0);
    }
}
