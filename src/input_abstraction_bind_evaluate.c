/* input_abstraction_bind_evaluate @0x8372E150 — HaloScript builtin wrapper for input_abstraction_bind; three strings. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void input_abstraction_bind(const char *device_specifier, const char *button_specifier, const char *control_specifier);

void input_abstraction_bind_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        input_abstraction_bind((const char *)arguments[0], (const char *)arguments[1], (const char *)arguments[2]);
        hs_return(thread_index, 0);
    }
}
