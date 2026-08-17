/* real_local_random @0x83685398 */
#include <stdint.h>

extern uint32_t *get_global_local_random_seed_address(void);
extern float real_seed_random(uint32_t *seed);

/* DEVIATION: decompiler double-return idiom; disasm: real_seed_random returns double, frsp to float */
float real_local_random(void)
{
    unsigned int *seed = get_global_local_random_seed_address();
    return real_seed_random(seed);
}
