/* update_cumulative @0x83763B78 — recompute a net_graph's cumulative bits-per-second figures from the
 * running bit totals and the elapsed time since cumulative_start_ms. When not yet started (start==0) the
 * rates are zeroed.
 * DEVIATION: the decompiler bailed ("local variable allocation has failed") on the soft-float __int64
 * juggling. Recovered from disassembly: elapsed_sec = elapsed_ms * 0.001; the divisor is elapsed_sec when
 * >= 1s else (1/elapsed_sec); inv = 1/divisor; each total (unsigned->float) is scaled by inv. */

#include <stdint.h>
#include "headers/net_graph.h"

extern uint32_t system_milliseconds(void);

void update_cumulative(net_graph *graph)
{
    unsigned int now = system_milliseconds();
    unsigned int start = graph->cumulative_start_ms;
    unsigned int elapsed_ms = now - start;
    float elapsed_sec = (float)elapsed_ms * 0.001f;

    if ( start )
    {
        float divisor = elapsed_sec;
        if ( divisor < 1.0f )
            divisor = 1.0f / elapsed_sec;
        float inv = 1.0f / divisor;
        graph->cumulative_bps_sent = (float)graph->cumulative_bit_sent_total * inv;
        graph->cumulative_bps_received = (float)graph->cumulative_bit_received_total * inv;
    }
    else
    {
        graph->cumulative_bps_sent = 0.0f;
        graph->cumulative_bps_received = 0.0f;
    }
}
