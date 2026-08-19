/* main_skip_cinematic_evaluate @0x8372C0A8 — HaloScript wrapper for (main_skip_cinematic): run the command, return nothing.
 * Deviation: main_skip_cinematic is void no-arg; the hs_* parameters are ABI residue passed by the dispatcher. */

#include <stdint.h>

extern void main_skip_cinematic(void);
extern void hs_return(int thread_index, int value);

void main_skip_cinematic_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    main_skip_cinematic();
    hs_return(thread_index, 0);
}
