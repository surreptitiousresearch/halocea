/* structure_lens_flares_place_evaluate @0x8372D308 — HaloScript builtin wrapper.
 *
 * DEVIATION: structure_lens_flares_place is a void no-argument builtin; the decompiler renders it as taking the evaluator
 * (function_index, thread_index, initialize) triple, so it is called that way here. */

#include <stdint.h>

extern void hs_return(int thread_index, int value);
extern void structure_lens_flares_place(void);

void structure_lens_flares_place_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    structure_lens_flares_place();
    hs_return(thread_index, 0);
}
