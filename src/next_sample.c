#include <stdint.h>
#include "headers/net_graph.h"

extern uint32_t system_milliseconds(void);

void next_sample(net_graph *const graph)
{
    graph->period_start_ms = system_milliseconds();
    graph->current_sample_data = 0;
}
