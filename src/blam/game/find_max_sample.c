/* find_max_sample @0x837633F0 — scan a net_graph's 320-sample ring buffer for the largest sample, starting the
 * running max at 1 (so an all-zero graph reports a max of 1) and the fallback index at 319. On a tie the
 * later (more recent) sample wins, since the comparison is >=. If lifetime is non-null, stores the winning
 * sample's index + 1 (its "age" in samples).
 *
 * DEVIATION: the compiler unrolled this into 5-wide blocks with pointer arithmetic; verified via disasm that
 * each of the 5 unrolled comparisons is the identical >= test applied in strict increasing index order, so
 * this is exactly equivalent to — and restored as — a plain sequential scan. */

#include "headers/net_graph.h"

int find_max_sample(net_graph *const graph, int *const lifetime)
{
    int max_value = 1;
    int max_index = 319;

    for ( int i = 0; i < 320; i++ )
    {
        if ( graph->samples[i] >= max_value )
        {
            max_value = graph->samples[i];
            max_index = i;
        }
    }

    if ( lifetime )
        *lifetime = max_index + 1;

    return max_value;
}
