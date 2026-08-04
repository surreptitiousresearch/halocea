/* hs_object_create_anew_containing_evaluate @0x83727488 — HaloScript builtin evaluator. Argument [0] = name substring (string). No return
 * value. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void hs_object_create_anew_containing(const char *name_string);
void hs_object_create_anew_containing_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    char **arguments = (char **)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments ) { hs_object_create_anew_containing(*arguments); hs_return(thread_index, 0); }
}
