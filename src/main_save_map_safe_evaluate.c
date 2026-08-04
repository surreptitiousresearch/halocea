/* main_save_map_safe_evaluate @0x8372C418 — HaloScript wrapper for (main_save_map_safe): run the command, return nothing.
 * Deviation: main_save_map_safe is void no-arg; the hs_* parameters are ABI residue passed by the dispatcher. */

#include <stdint.h>

extern void main_save_map_safe(void);
extern void hs_return(uint16_t thread_index, int value);

void main_save_map_safe_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    main_save_map_safe();
    hs_return(thread_index, 0);
}
