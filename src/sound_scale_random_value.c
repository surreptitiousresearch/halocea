#include <stdint.h>
extern uint32_t *get_global_local_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);

float sound_scale_random_value(float base_lower_bound, float base_upper_bound,
        float lower_bound_modifier, float upper_bound_modifier, float scale)
{
    unsigned int *seed = get_global_local_random_seed_address();
    float base_value = real_seed_random_range(seed, base_lower_bound, base_upper_bound);

    return base_value * ((upper_bound_modifier - lower_bound_modifier) * scale + lower_bound_modifier);
}
