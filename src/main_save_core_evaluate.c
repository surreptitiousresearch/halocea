/* main_save_core_evaluate @0x8372E5C0 — HaloScript wrapper for (main_save_core): run the command, return nothing.
 * Deviation: main_save_core is void no-arg; the hs_* parameters are ABI residue passed by the dispatcher. */

#include <stdint.h>

extern void main_save_core(void);
extern void hs_return(uint16_t thread_index, int value);

void main_save_core_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    main_save_core();
    hs_return(thread_index, 0);
}
