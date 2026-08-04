#include <stdint.h>
#include "headers/blam_data_globals.h"
/* get_random_seed @0x83722E10 — return the current global random seed. */


uint32_t get_random_seed(void)
{
    return global_random_seed;
}
