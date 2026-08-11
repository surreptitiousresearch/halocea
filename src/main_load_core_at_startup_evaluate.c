/* main_load_core_at_startup_evaluate @0x8372C578 — HaloScript wrapper for (main_load_core_at_startup): run the command, return nothing.
 * Deviation: main_load_core_at_startup is void no-arg; the hs_* parameters are ABI residue passed by the dispatcher. */

#include <stdint.h>

extern void main_load_core_at_startup(void);
extern void hs_return(int thread_index, int value);

void main_load_core_at_startup_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    main_load_core_at_startup();
    hs_return(thread_index, 0);
}
