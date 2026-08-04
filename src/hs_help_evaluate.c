/* hs_help_evaluate @0x83730368 — HaloScript builtin evaluator. Argument [0] = function name (string). No
 * return value. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void hs_help(const char *function_name);
void hs_help_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    char **arguments = (char **)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments ) { hs_help(*arguments); hs_return(thread_index, 0); }
}
