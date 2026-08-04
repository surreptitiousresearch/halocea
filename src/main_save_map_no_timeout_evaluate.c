/* main_save_map_no_timeout_evaluate @0x8372C488 — HaloScript wrapper for (main_save_map_no_timeout): run the command, return nothing.
 * Deviation: main_save_map_no_timeout is void no-arg; the hs_* parameters are ABI residue passed by the dispatcher. */

#include <stdint.h>

extern void main_save_map_no_timeout(void);
extern void hs_return(uint16_t thread_index, int value);

void main_save_map_no_timeout_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    main_save_map_no_timeout();
    hs_return(thread_index, 0);
}
