/* main_print_version_evaluate @0x8372BE08 — HaloScript wrapper for (main_print_version).
 * Deviation: main_print_version is void no-arg; hs_* parameters are ABI residue. */

#include <stdint.h>

extern void main_print_version(void);
extern void hs_return(int thread_index, int value);

void main_print_version_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    main_print_version();
    hs_return(thread_index, 0);
}
