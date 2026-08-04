#include <stdint.h>
#include "headers/blam_data_globals.h"


uint32_t *get_global_random_seed_address(void)
{
    return &global_random_seed;
}
