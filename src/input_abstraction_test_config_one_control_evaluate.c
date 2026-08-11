/* input_abstraction_test_config_one_control_evaluate @0x8372DFC8 — HaloScript builtin wrapper; one string. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void input_abstraction_test_config_one_control(const char *control_specifier);

void input_abstraction_test_config_one_control_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        input_abstraction_test_config_one_control((const char *)arguments[0]);
        hs_return(thread_index, 0);
    }
}
