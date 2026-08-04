#include <math.h>
#include <stdint.h>

extern float real_seed_random(uint32_t *seed);
extern uint32_t *get_global_random_seed_address(void);

void periodic_function_build_variable_period_x_table(float *x_table)
{
    float accumulated = 0.0f;
    int   i;

    for (i = 0; i < 1024; i = (int16_t)(i + 1))
    {
        float fi = (float)i;
        float r0, c1, r2, c3, r4, c5, r6;

        x_table[i] = accumulated;

        r0 = real_seed_random(get_global_random_seed_address());
        c1 = (float)cos(fi * 0.025157286f);
        r2 = real_seed_random(get_global_random_seed_address());
        c3 = (float)cos(fi * 0.03129321f);
        r4 = real_seed_random(get_global_random_seed_address());
        c5 = (float)cos(fi * 0.044792242f);
        r6 = real_seed_random(get_global_random_seed_address());

        accumulated = accumulated
                    + ((r0 + 1.0f) * 0.25f)
                    + ((c1 + 1.0f) * r2)
                    + (r6 * (c5 + 1.0f))
                    + ((c3 + 1.0f) * r4);
    }

    for (i = 0; i < 1024; i = (int16_t)(i + 1))
        x_table[i] = x_table[i] * (1.0f / accumulated);
}
