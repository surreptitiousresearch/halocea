/* real_random @0x836B10B8 */
#include <stdint.h>
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random(uint32_t *seed);

/* DEVIATION: decompiler double-return idiom; disasm: real_seed_random returns double, frsp to float */
float real_random(void)
{
    unsigned int *seed = get_global_random_seed_address();
    return real_seed_random(seed);
}
