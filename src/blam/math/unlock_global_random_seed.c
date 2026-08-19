#include "headers/blam_data_globals.h"
/* unlock_global_random_seed @0x83722DE0 — releases one level of the global random-seed lock. */


void unlock_global_random_seed(void)
{
    --random_seed_lock_count;
}
