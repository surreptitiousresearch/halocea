/* main_load_core_evaluate @0x8372E588 — HaloScript wrapper for (main_load_core): run the command, return nothing.
 * Deviation: main_load_core is void no-arg; the hs_* parameters are ABI residue passed by the dispatcher. */

#include <stdint.h>

extern void main_load_core(void);
extern void hs_return(int thread_index, int value);

void main_load_core_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    main_load_core();
    hs_return(thread_index, 0);
}
