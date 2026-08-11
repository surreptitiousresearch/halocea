/* message_delta_metrics_clear_evaluate @0x8372D240 — HaloScript builtin wrapper.
 *
 * DEVIATION (2026-07-31): message_delta_metrics_clear is a void no-argument builtin (disasm: no arg regs
 * read before written; tail-calls metrics_clear); the decompiler renders it as taking the evaluator
 * (function_index, thread_index, initialize) triple, but it is declared and called zero-arg here. */

#include <stdint.h>

extern void hs_return(int thread_index, int value);
extern void message_delta_metrics_clear(void);

void message_delta_metrics_clear_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    (void)function_index;
    (void)initialize;
    message_delta_metrics_clear();
    hs_return(thread_index, 0);
}
