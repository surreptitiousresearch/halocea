/* console_func_sleep_evaluate @0x8372E330 — HaloScript builtin evaluator. Argument block: [0] = value. No return value. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void console_func_sleep(unsigned int milliseconds);

void console_func_sleep_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        console_func_sleep(arguments[0]);
        hs_return(thread_index, 0);
    }
}
