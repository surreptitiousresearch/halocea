/* test_input_device_defaults_find_evaluate @0x8372DF80 — HaloScript builtin wrapper for test_input_device_defaults_find; one string. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void test_input_device_defaults_find(const char *id_string);

void test_input_device_defaults_find_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    const char **arguments = (const char **)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        test_input_device_defaults_find(*arguments);
        hs_return(thread_index, 0);
    }
}
