/* hs_not_evaluate @0x83727138 — HaloScript builtin evaluator. Argument [0] = boolean. Returns the logical
 * negation packed into the high byte of the result word. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern uint8_t hs_not(uint8_t value);
void hs_not_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int result = 0;
    unsigned char *arguments = (unsigned char *)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments ) { *((unsigned char *)&result + 3) = hs_not(*arguments); hs_return(thread_index, result); }
}
