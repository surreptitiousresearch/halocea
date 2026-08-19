/* net_graph_clear_evaluate @0x8372D128 — HaloScript builtin wrapper.
 *
 * DEVIATION: net_graph_clear is a void no-argument builtin; the decompiler renders it as taking the evaluator
 * (function_index, thread_index, initialize) triple, so it is called that way here. */

#include <stdint.h>

extern void hs_return(int thread_index, int value);
extern void net_graph_clear(void);

void net_graph_clear_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    net_graph_clear();
    hs_return(thread_index, 0);
}
