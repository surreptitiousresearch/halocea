/* main_reset_map_evaluate @0x8372BC68 — HaloScript wrapper for (main_reset_map): run the command, return nothing.
 * Deviation: main_reset_map is void no-arg; the hs_* parameters are ABI residue passed by the dispatcher. */

#include <stdint.h>

extern void main_reset_map(void);
extern void hs_return(uint16_t thread_index, int value);

void main_reset_map_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    main_reset_map();
    hs_return(thread_index, 0);
}
