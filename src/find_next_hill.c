#include <stdint.h>
#include "headers/blam_data_globals.h"
/* find_next_hill @0x8382BAF8 — king-of-the-hill: picks a random hill (from `king_engine_hills`, sized by
 * `king_engine_hill_count`) different from `hill_id`, scanning forward from a random starting index and wrapping
 * around the hill count. Returns an uninitialized value (matching the original) if there are no hills or
 * every hill scanned equals `hill_id`. */

extern __int16 king_engine_hill_count;

extern uint32_t *get_global_random_seed_address(void);
extern int16_t seed_random_range(uint32_t *seed, int16_t lower_bound, int16_t upper_bound);

int find_next_hill(int hill_id)
{
    int result;
    __int16 hill_count = king_engine_hill_count;
    unsigned int *seed = get_global_random_seed_address();
    __int16 start_index = seed_random_range(seed, 0, hill_count);

    if ( hill_count <= 0 )
        return result;

    __int16 candidate_index = start_index;
    __int16 scanned = 0;
    while ( 1 )
    {
        result = king_engine_hills[(__int16)(candidate_index % hill_count)];
        if ( hill_id != result )
            break;
        ++scanned;
        if ( scanned >= hill_count )
            return result;
        ++candidate_index;
    }

    return result;
}
