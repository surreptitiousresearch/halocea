/* errors_overflow_suppression_enable_evaluate @0x8372D2C0 — HaloScript builtin evaluator. Argument block:
 * [0] = boolean. No return value. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void errors_overflow_suppression_enable(uint8_t overflow_suppression);

void errors_overflow_suppression_enable_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    unsigned char *arguments = (unsigned char *)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        errors_overflow_suppression_enable(*arguments);
        hs_return(thread_index, 0);
    }
}
