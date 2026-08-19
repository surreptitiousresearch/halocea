/* sv_parameters_dump_evaluate @0x8372E2B0 — HaloScript builtin wrapper for the server command sv_parameters_dump.
 *
 * DEVIATION (2026-07-31): sv_parameters_dump is a void no-argument console builtin (disasm: no arg regs
 * read before written; tail-calls message_delta_parameters_protocol_dump_to_config_file); the decompiler
 * renders it as taking the evaluator's (function_index, thread_index, initialize) triple, but it is
 * declared and called zero-arg here. */

#include <stdint.h>

extern void hs_return(int thread_index, int value);
extern void sv_parameters_dump(void);

void sv_parameters_dump_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    (void)function_index;
    (void)initialize;
    sv_parameters_dump();
    hs_return(thread_index, 0);
}
