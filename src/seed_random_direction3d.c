/* seed_random_direction3d @0x83723080 — pick a uniformly random unit direction from the precomputed
 * random direction table, advancing the seed (LCG). */

#include <stdint.h>
#include "headers/random_math_globals.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"


real_vector3d *seed_random_direction3d(uint32_t *seed, real_vector3d *direction)
{
    real_vector3d *random_direction_table = random_math_globals.random_direction_table;
    int random_direction_table_size = random_math_globals.random_direction_table_size;
    uint32_t next_seed = 1664525u * *seed + 1013904223u;

    *seed = next_seed;
    /* casts model srwi 16 / extsh in the index computation */
    *direction = random_direction_table[(int16_t)((uint32_t)(((uint16_t)(next_seed >> 16)) * random_direction_table_size) >> 16)];
    return direction;
}
