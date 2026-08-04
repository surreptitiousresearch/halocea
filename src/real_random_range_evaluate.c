/* real_random_range_evaluate @0x83727A78 — HaloScript builtin wrapper for a seeded real range; (real, real)
 * returning a real drawn from the global random seed. */

#include <stdint.h>
extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);

void real_random_range_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    float *arguments = (float *)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        float upper = arguments[1];
        float lower = arguments[0];
        int result;
        *(float *)&result = real_seed_random_range(get_global_random_seed_address(), lower, upper);
        hs_return(thread_index, result);
    }
}
