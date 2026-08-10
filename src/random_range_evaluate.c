/* random_range_evaluate @0x83727A20 — HaloScript builtin wrapper for a seeded integer range; (short, short)
 * returning a short drawn from the global random seed. */

#include <stdint.h>
extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern uint32_t *get_global_random_seed_address(void);
extern int16_t seed_random_range(uint32_t *seed, int16_t lower_bound, int16_t upper_bound);

void random_range_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int result = 0;   /* DEVIATION: `stw r11, 0x50(r1)` @0x83727A34 zeroes the WHOLE word — the halfword */
    /* form left slot bytes 2-3 uninitialised; `sth 0x50(r1)` @0x83727A5C puts the short at slot halfword 0. */
    int16_t *arguments = (int16_t *)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        int16_t upper = arguments[2];
        int16_t lower = arguments[0];
        *(int16_t *)&result = seed_random_range(get_global_random_seed_address(), lower, upper);
        hs_return(thread_index, result);
    }
}
