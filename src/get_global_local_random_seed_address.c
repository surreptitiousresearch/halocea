#include <stdint.h>
#include "headers/blam_data_globals.h"
/* get_global_local_random_seed_address @ 0x83722E18 — address of the local (non-deterministic)
 * random seed used for cosmetic randomness like sound pitch/skip. */


uint32_t *get_global_local_random_seed_address(void)
{
    return &global_local_random_seed;
}
