/* input_abstraction_print_binds_evaluate @0x8372E1F0 — HaloScript builtin wrapper.
 *
 * DEVIATION: input_abstraction_print_binds is a void no-argument builtin; the decompiler renders it as taking
 * the evaluator (function_index, thread_index, initialize) triple, so it is called that way here. */

#include <stdint.h>

extern void hs_return(uint16_t thread_index, int value);
extern void input_abstraction_print_binds(void);

void input_abstraction_print_binds_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    input_abstraction_print_binds();
    hs_return(thread_index, 0);
}
