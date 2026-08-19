/* sv_name_evaluate @0x8372EA40 — HaloScript builtin wrapper for the server command sv_name, which takes a variable
 * number of string arguments collected by hs_macro_function_strings_evaluate. */

#include <stdint.h>

extern void hs_return(int thread_index, int value);
extern uint8_t hs_macro_function_strings_evaluate(int thread_index, uint8_t initialize, int *count_out, const char ***strings_out);
extern void sv_name(int count, char **strings);

void sv_name_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int argument_count = 0;
    char **arguments = nullptr;
    if ( hs_macro_function_strings_evaluate(thread_index, initialize, &argument_count, (const char ***)&arguments) )
    {
        sv_name(argument_count, arguments);
        hs_return(thread_index, 0);
    }
}
