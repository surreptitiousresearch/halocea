/* hs_macro_function_evaluate @0x8368E970 — shared entry point for "macro" HaloScript builtins: evaluates the
 * function's formal arguments (from the function table) and returns a pointer to the evaluated argument
 * block, or null if evaluation is not yet complete. Almost every builtin evaluator calls this first. */

#include <stdint.h>
#include "headers/hs_function_definition.h"
#include "headers/blam_data_globals.h"

extern int * hs_arguments_evaluate(int thread_index, int16_t formal_parameter_count, const int16_t *formal_parameters, uint8_t initialize);

int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    return hs_arguments_evaluate(thread_index,
                                 hs_function_table[function_index]->formal_parameter_count,
                                 hs_function_table[function_index]->formal_parameters,
                                 initialize);
}
