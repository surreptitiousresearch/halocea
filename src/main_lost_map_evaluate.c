/* main_lost_map_evaluate @0x8372C2C0 — HaloScript wrapper for (main_lost_map): run the command, return nothing.
 * Deviation: main_lost_map is void no-arg; the hs_* parameters are ABI residue passed by the dispatcher. */

#include <stdint.h>

extern void main_lost_map(void);
extern void hs_return(int thread_index, int value);

void main_lost_map_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    main_lost_map();
    hs_return(thread_index, 0);
}
