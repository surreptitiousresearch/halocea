#include "headers/blam_data_globals.h"
/* lock_global_random_seed @0x83722DC8 — increments the global random-seed lock counter so the
 * deterministic random seed is held stable across a critical section of the tick. */


void lock_global_random_seed(void)
{
    ++random_seed_lock_count;
}
