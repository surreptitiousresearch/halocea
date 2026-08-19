/* input_abstraction_unbind_evaluate @0x8372E1A0 — HaloScript builtin wrapper for input_abstraction_unbind; two strings. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void input_abstraction_unbind(const char *device_specifier, const char *button_specifier);

void input_abstraction_unbind_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        input_abstraction_unbind((const char *)arguments[0], (const char *)arguments[1]);
        hs_return(thread_index, 0);
    }
}
