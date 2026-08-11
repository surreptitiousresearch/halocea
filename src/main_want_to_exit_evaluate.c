/* main_want_to_exit_evaluate @0x8372E498 — HaloScript wrapper for (main_want_to_exit): run the command, return nothing.
 * Deviation: main_want_to_exit is void no-arg; the hs_* parameters are ABI residue passed by the dispatcher. */

#include <stdint.h>

extern void main_want_to_exit(void);
extern void hs_return(int thread_index, int value);

void main_want_to_exit_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    main_want_to_exit();
    hs_return(thread_index, 0);
}
